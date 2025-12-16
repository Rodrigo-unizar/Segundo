#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int crear = 0;

void fcap(int n){
    if(n == SIGUSR1){
        crear ++;
        signal(SIGUSR1, fcap);
    }
    if(n == SIGALRM){
        signal(14, fcap);
    }

}

int main(){
    int pelota = 1;
    signal(SIGUSR1, fcap);
    signal(14, fcap);
    int fdhp[2];
    int fdph[2];
    pipe(fdph);
    pipe(fdhp);
    printf("%d\n", getpid());
    switch (fork())
    {
    case 0://hijo 
        close(0);
        dup(fdph[0]);
        close(1);
        dup(fdhp[1]);
        close(3);
        close(4);
        close(5);
        close(6);
        while(read(0, &pelota, sizeof(int)) > 0){
            write(2, "Soy el hijo y me llega la pelota: ",35 );
            char buf[32];
            int len = sprintf(buf, "%d", pelota);
            write(2, buf, len);
            write(2, "\n", 1);
            pelota++;
            alarm(1);
            pause();
            write(1, &pelota, sizeof(pelota));
        }
        exit(1);
    default://padre
        close(0);
        dup(fdhp[0]);
        close(1);
        dup(fdph[1]);
        close(3);
        close(4);
        close(5);
        close(6);
        write(1, &pelota, sizeof(pelota));
        while(read(0, &pelota, sizeof(int)) > 0){
            if(crear){
                crear = 0;
                
                int fd2[2];
                pipe(fd2);
                int pid = fork();

                if(pid == 0){
                    close(0);
                    dup(fd2[0]);
                    while(read(0, &pelota, sizeof(int)) > 0){
                        write(2, "Soy el hijo y me llega la pelota: ",35 );
                        char buf[32];
                        int len = sprintf(buf, "%d", pelota);
                        write(2, buf, len);
                        write(2, "\n", 1);
                        pelota++;
                        alarm(1);
                        pause();
                        write(1, &pelota, sizeof(pelota));
                    }
                    exit(1);
                }
                close(1);
                dup(fd2[1]);
            }
            write(2, "Soy el padre y me llega la pelota: ",36 );
            char buf[32];
            int len = sprintf(buf, "%d", pelota);
            write(2, buf, len);
            write(2, "\n", 1);
            pelota++;
            alarm(1);
            pause();
            write(1, &pelota, sizeof(pelota));
            
        }
    }
}