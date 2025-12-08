#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void fcaptura(int n){
    signal(SIGALRM, fcaptura);
}
void ffinal(int n){
    printf("Finalizando proceso con pid %d \n", getpid());
    exit(0);
}

int main(){
    void (* syshandler)(int);
    syshandler = signal(SIGALRM, fcaptura);
    syshandler = signal(SIGINT, ffinal);
    int i = 1;
    switch(fork()){
        case -1:
            fprintf(stderr, "Error al crear el hijo");
            exit(1);
        case 0:
            // Proceso hijo
            i++;
            switch(fork()){
                case -1:
                    fprintf(stderr, "Error al crear el nieto");
                    exit(1);
                case 0:
                    // P3
                    i++;
                    alarm(i);
                    pause();
                    printf("P3 con pid %d, segundo %d \n", getpid(), i);
                    while(1){
                        alarm(3);
                        pause();
                        i = i+3;
                        printf("P3 con pid %d, segundo %d \n", getpid(), i);    
                    }
                    exit(0);
                default:
                    // P2
                    alarm(i);
                    pause();
                    printf("P2 con pid %d, segundo %d \n", getpid(), i);  
                    while(1){
                        alarm(3);
                        pause();
                        i = i+3;
                        printf("P2 con pid %d, segundo %d \n", getpid(), i);    
                    }
                    wait(NULL);
                    exit(0);
            }
        default:
            // P1
            alarm(i);
            pause();
            printf("P1 con pid %d, segundo %d \n", getpid(), i);
            while(1){
                alarm(3);
                pause();
                i = i+3;
                printf("P1 con pid %d, segundo %d \n", getpid(), i);    
            }
            wait(NULL);
            exit(0);
    }

}