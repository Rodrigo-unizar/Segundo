#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



int main(int argc, char *argv[]){

    if(argc < 3){
        fprintf(stderr, "Numero de parametros incorrecto\n");
        exit(1);
    }

    int m = atoi(argv[2]);
    int n = atoi(argv[1]);  
    int a = 'a';

    while(a - 'a'< n){
        switch(fork()){
            case -1:
                exit(1);
            case 0:
                // Proceso hijo
                for(int i = 0; i < m; i++) {
                    fprintf(stderr,"%c \n",a);   //función de biblioteca
                }
                exit(0);
            default:
                // Proceso Hijo
                wait(NULL);
                a++;
                
        }
        
    }

}



