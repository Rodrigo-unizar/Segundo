#include <threads.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    if(argc != 3){
        write(1, "Numero de parametros incorrectos", 33);
        exit(0);
    }

    int n = atoi(argv[2]);
    int fd = open(argv[1], 0);
    long fsize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    
    int total = fsize / sizeof(int);
    int tramo = total / n;
    int p[n][2];
    for(int  i = 0; i < n; i++){
        pipe(p[i]);
        int pid = fork();
        if(pid == 0){
            fd = open(argv[1], 0);
            lseek(fd, tramo * i * sizeof(int) , SEEK_SET);
            int leido;
            int suma = 0;
            for(int j = 0; j < tramo; j++){
                read(fd, &leido, sizeof(int));
                suma += leido;
            }
            write(p[i][1], &suma, sizeof(int));
            close(p[i][0]);
            exit(0);
        }
        close(p[i][1]);
    }
    long resFin = 0;
    int parcial = 0;
    char buf[32];
    int len;

    for(int  i = 0; i < n; i++){
        read(p[i][0], &parcial, sizeof(int));
        resFin += parcial;
    }
    int resto = total - tramo * n;
    int resul = 0;
    lseek(fd, tramo * n * sizeof(int), 0);
    while(resto != 0){
        read(fd, &resul, sizeof(int));
        resFin += resul;
        resto--;
    }

    len = sprintf(buf, "%d", resFin);
    write(1, "El numero final es: ", 21);
    write(2, buf, len);
    write(1,"\n", 1);
}