#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    
    if(argc != 3){
        write(2, "Numero de parametros incorrectos", 33);
        return 1;
    }
    char buffer[BUFSIZ];
    ssize_t num_Bytes;
    int pf[2];
    pipe(pf);
    switch (fork())
    {
    case -1:
        write(2, "Error al hacer fork()", 22);
        exit(1);
    case 0:
        int fich = open(argv[1], 0);
        
        close(pf[0]);
        while ((num_Bytes = read(fich, buffer, sizeof(buffer) - 1)) > 0){
            write(pf[1], buffer, num_Bytes);
        }
        close(pf[1]);
        close(fich);
        unlink(argv[1]);
        exit(0);
    default:
        int fich2 = open(argv[2], 1 | O_CREAT | O_TRUNC );
        close(pf[1]);
        while ((num_Bytes = read(pf[0], buffer, sizeof(buffer) - 1)) > 0){
            write(fich2, buffer, num_Bytes);
        }
        exit(0);
    }
}