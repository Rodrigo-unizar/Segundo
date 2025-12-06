#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int pf[2];
    pipe(pf);
    
    if(fork() == 0){
        close(pf[0]);
        close(1);   
        dup(pf[1]);
        close(pf[1]);
        execlp(argv[1], argv[1], NULL);
        perror("execlp");
        exit(1);
    }

    if(fork() == 0){
        close(pf[0]);
        close(1);   
        dup(pf[1]);
        close(pf[1]);
        execlp(argv[2], argv[2], NULL);
        perror("execlp");
        exit(1);
    }
     //Padre
    close(pf[1]);
    close(0);   
    dup(pf[0]);
    close(pf[0]);
    execlp(argv[3], argv[3], NULL);
    perror("execlp");
    exit(1);
    
}