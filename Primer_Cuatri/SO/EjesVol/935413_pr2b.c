#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void f(int s){
    fprintf(stderr,"-");
}

int main(){
    int pid;
    sigset_t mask, vmask, nmask;
    struct sigaction act;

    act.sa_handler=f;
    sigemptyset(&act.sa_mask); //inicializacion del sigset
    act.sa_flags=0; 
    sigaction(SIGUSR1, &act, NULL); //Cambiamos la accion de la senal SIGUSR1 a la funcion f

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, &vmask); 
    
    if((pid=fork())==0){
        pid=getppid();
        while(1){
            fprintf(stderr,"h");
            kill(pid,SIGUSR1);
            sigsuspend(&nmask);
        }
    }
    else{
        sigsuspend(&nmask);
        while(1){
            fprintf(stderr,"p");
            kill(pid,SIGUSR1);
            sigsuspend(&nmask);
        }
    }
}
