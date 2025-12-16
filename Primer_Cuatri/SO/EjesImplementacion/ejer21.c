#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char* argv[]){
    if(argc != 2){
        write(2, "Numero de parametros incorrecto", 32);
        exit(2);
    }
    ssize_t numBytes;
    char buff[1024];
    int fd = open(argv[1], 1 |O_CREAT | O_TRUNC, 0666);
    while((numBytes = read(0, buff, sizeof(buff))) > 0){
        write(1, buff, numBytes);
        write(fd, buff, numBytes);
    }
}