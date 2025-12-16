#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


struct timeval iniCrono;
struct timeval finCrono;
int modo = 0;

void fcap(int n){
    if(n == 2){
        if(modo == 0){
            gettimeofday(&iniCrono, NULL);
            modo = 1;
        }else{
            gettimeofday(&finCrono, NULL);
            modo = 0;
            finCrono.tv_sec  -= iniCrono.tv_sec;
            finCrono.tv_usec -= iniCrono.tv_usec;
            if (finCrono.tv_usec < 0) {
                finCrono.tv_sec--;
                finCrono.tv_usec += 1000000;
            }
            printf("Han transcurrido %ld.%06ld segundos\n",
                   finCrono.tv_sec, finCrono.tv_usec);
        }
        signal(2, fcap);
    }
    if (n == 14) {
        write(1, "Fin del minuto\n", 15);
        exit(0);
    }
}



int main(){
    signal(2, fcap);
    signal(14, fcap);

    alarm(60);
    while (1) pause();
}