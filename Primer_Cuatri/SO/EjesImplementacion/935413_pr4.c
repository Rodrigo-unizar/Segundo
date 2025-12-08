#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

int modo = 1;

void fcaptura(int n){
    modo = ((modo +1)%3);
    signal(SIGUSR1, fcaptura);
}

struct params {
    int fdredir;
    int lpipe;
};

void *redireccionar(void* arg){
    struct params *p = arg;
    int fdredir = p->fdredir;
    int lpipe   = p->lpipe;

    char buffer[1024];
    ssize_t bytesleidos;

    while ((bytesleidos = read(lpipe, buffer, sizeof(buffer))) > 0){
        write(fdredir, buffer, bytesleidos);
    }
    return NULL;
}



int main(int argc, char* argv[]){
    pthread_t tid[2];
    signal(SIGUSR1, fcaptura);
    char buffer[1024];
    int salEst[2];
    int salFic[2];
    ssize_t numBytes;

    if (argc < 4){
        write(2, "Numero de parametros incorrecto\n", 32);
        return 1;
    }

    int fdredir;
    fdredir = open(argv[2], 1 | O_CREAT | O_TRUNC);
    fprintf(stderr, "PID del proceso %d\n", getpid());
    modo = atoi(argv[1]) - 1;
    
    int salexec[2];
    pipe(salexec);
    switch (fork()){
        case 0: //Es el hijo y se va a encargar de ejecutar el comando
            close(1);
            dup(salexec[1]);
            close(salexec[0]);
            close(fdredir);
            close(salexec[1]);
            execvp(argv[3], &argv[3]);
            fprintf(stderr, "Error en la ejecucion del comando\n");
            exit(1);
            
        default: //Es el padre y se va a encargar de "redirigir" la salida
            close(salexec[1]);
            pipe(salEst);
            pipe(salFic);
            struct params s1 = {1, salEst[0]};
            pthread_create(&tid[0], NULL,redireccionar,&s1);
            struct params s2 = {fdredir, salFic[0]};
            pthread_create(&tid[1], NULL,redireccionar, &s2);
            while ((numBytes = read(salexec[0], buffer, sizeof(buffer))) > 0){
                switch (modo + 1){
                    case 1:
                        write(salEst[1], buffer, numBytes);
                        break;
                    case 2:
                        write(salFic[1], buffer, numBytes);
                        break;
                    case 3:
                        write(salEst[1], buffer, numBytes);
                        write(salFic[1], buffer, numBytes);
                        break;
                }
            }
            close(salEst[1]);
            close(salFic[1]);
            pthread_join(tid[0], NULL);
            pthread_join(tid[1], NULL);
            wait(NULL); 
    } 
    
    return 0;
}