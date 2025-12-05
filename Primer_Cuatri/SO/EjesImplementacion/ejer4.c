#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


int main() {
    signal(9, 0);
    int salexec[2];
    pipe(salexec);

    switch (fork()) {
        case -1:
            perror("Error en fork");
            exit(EXIT_FAILURE);
        case 0: // Hijo
            close(salexec[0]);
            close(1);
            dup(salexec[1]);
            close(salexec[1]);
            execlp("ps", "ps", "-e ", NULL);
            perror("Error en execlp");
            exit(EXIT_FAILURE);
        default: // Padre
            close(salexec[1]);
            char buffer[1024];
            ssize_t bytesRead;
            while ((bytesRead = read(salexec[0], buffer, sizeof(buffer) - 1)) > 0) {
                
            }
            close(salexec[0]);
    }
    kill(pid, 9);
    return 0;
}