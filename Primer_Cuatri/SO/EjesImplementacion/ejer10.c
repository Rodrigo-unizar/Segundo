#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


int main(int argc, char* argv[]){
    /*if(argc != 2){
        fprintf(stderr, "Numero de parametros incorrecto\n ./programa nombre_de_directorio");
        exit(1);
    }*/
    int fd[2];
    pipe(fd);

    switch(fork()){
        case -1:
            fprintf(stderr, "Error en el fork");
            exit(1);
        case 0: //Hijo
            close(1);
            dup(fd[1]);
            close(fd[0]);
            close(fd[1]);
            execlp("ls","ls", "-l", NULL );
            write(2, "Error", 5);
            exit(1);
        default:
            close(fd[1]);
            close(0);
            dup(fd[0]);
            close(fd[0]);
            char* lineas[100];
            char buffer[4096];
            int i = 0;
            fgets(buffer, sizeof(buffer), stdin);; //primera line total x
            while(fgets(buffer, sizeof(buffer), stdin) != NULL && i < 100){
                lineas[i] = strdup(buffer);
                i++;
            }

            int numfich = 0;
            int numBytes = 0;
            char* comando[10];
            for(int j = 0; j < i; j ++){
                comando[0] = strtok(lineas[j], " \t");
                for(int k = 1; (comando[k] = strtok(NULL, " \t")) != NULL; k++);
                if(atoi(comando[1]) == 1){
                    numfich ++;
                    numBytes += atoi(comando[4]);
                }
            }

            printf("Hay %d ficheros que ocupan %d Bytes", numfich, numBytes);

    }
}