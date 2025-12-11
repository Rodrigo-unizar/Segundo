#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){

    if(argc != 3){
        write(2, "Numero de parametros incorrecto",32 );
    }

    int fd[2];
    pipe(fd);
    int fesc = open(argv[2], 1 | O_CREAT | O_TRUNC, 0666);
    for(int i = 0; i < atoi(argv[1]);i++){
        switch (fork())
        {
        case 0:
            close(fd[0]);
            int pid = getpid();
            write(fd[1], &pid, sizeof(int));
            exit(0);
        default:
            
            int pidhijo;
            read(fd[0], &pidhijo, sizeof(int));
            char buffer[64];
            int len = snprintf(buffer, sizeof(buffer), "%d\n", pidhijo);
            write(fesc, buffer, len);
        }
    }
    close(fd[0]);
    close(fesc);

}