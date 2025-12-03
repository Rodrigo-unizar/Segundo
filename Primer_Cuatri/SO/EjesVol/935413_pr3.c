#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc,char* argv[]){
    static char fichero[5000];
    char* argt[5];
    int fd3;
    char *com1[100];
    char *com2[100];
    char fent[1000], fsal[1000], ferr[1000];

    if(argc != 2){
        write(2,"Numero de parametros incorrecto\n",32);
        return 1;
    }
    
    fd3 = open(argv[1],0 | O_CREAT);//FD 3
    read(fd3, fichero, sizeof(fichero));
    close(fd3);

    argt[0] = strtok(fichero, "\n");
    for(int i = 1; i < 5; i++) {
        argt[i] = strtok(NULL, "\n");
    }
    
    com1[0] = strtok(argt[0], " \t");
    for(int i = 1; (com1[i] = strtok(NULL, " \t")) != NULL; i++);

    com2[0] = strtok(argt[1], " \t");
    for(int i = 1; (com2[i] = strtok(NULL, " \t")) != NULL; i++);

    strcpy(fent, argt[2]);
    strcpy(fsal, argt[3]);
    strcpy(ferr, argt[4]);

    
    int comProc[2];
    pipe(comProc); // FD 3 para leer, FD 3 para escribir

    close(2);//Cerramos stderr
    open(ferr, 1 | O_CREAT |O_TRUNC); //ponemos como salida de errores el fichero de errores

    switch(fork()){
        case -1:
            write(2,"Error en fork\n",15);
            return 1;
        case 0: //Hijo
            close(0); //Cerramos stdin
            dup(comProc[0]); //Duplicamos el extremo de lectura del pipe en el stdin
            close(3);//cerramos la pipe
            close(4);//cerramos la pipe
            close(1);//cerramos stdout
            open(fsal, 1 | O_CREAT |O_TRUNC);//ponemos como salida standar el fichero de salida
            execvp(com2[0], &com2[0]);
            write(2,"Error en execlp\n",16);
            return 1;

        default: //Padre
            close(1);//Cerramos stdout
            dup(comProc[1]);//Duplicamos el extremo de escritura del pipe en el stdout
            close(3);//cerramos la pipe
            close(4);//cerramos la pipe
            close(0);//Cerramos stdin
            open(fent, 0);
            execvp(com1[0], &com1[0]);
            write(2, "Error en execlp\n", 16);
            exit(1);

            
    }

}