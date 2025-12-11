//este codigo crea 3 proceos + el padre sirve para comprobar el codigo del ejer4


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    printf("Proceso inicial: PID=%d, PPID=%d\n", getpid(), getppid());

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        printf("Fork %d -> Soy PID=%d, PPID=%d\n", i+1, getpid(), getppid());
    }

    // Después de los 3 forks, todos los procesos llegan aquí
    printf("PID=%d quedando en pause()\n", getpid());  
    pause();   // espera señal para terminar

    return 0;
}
