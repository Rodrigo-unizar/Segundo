#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h> 
#include <stdbool.h> 
#include <stdlib.h>

int main(int argc, char* argv[]){
    int i = 1;
    bool sinc = false;

    while(*argv[i] != '1' && *argv[i] != '0'){
        i++;
    }
    if(*argv[i] == '1'){
        sinc = true;
    }
    argv[i] = NULL;
    int pid;

    switch(pid = fork()){
        case 0:
            execvp(argv[1], &argv[1]);
        default:
            if(sinc){
                wait(NULL);
            }
            printf("%d", pid);
    }
}