#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void fcaptura(){

}

int main(){
    int pelota = 0;
    signal(14, fcaptura());
    int cont = 0;
    int p2h[2]; //tuberia del padre al hijo
    int h2p[2]; //tuberia del hijo al padre
    pipe(p2h); 
    pipe(h2p);

    switch(fork()){
        case -1:
            perror("Error en el fork");
            exit(EXIT_FAILURE);
        case 0: //Hijo
            close(p2h[1]); //Cierro el extremo de escritura del padre al hijo 
            close(h2p[0]); //Cierro el extremo de lectura del hijo al padre
            while(pelota < 100){

                cont = read(p2h[0], &pelota, sizeof(pelota)); //Leo la pelota enviada por el padre
                printf("Hijo recibe: %d\n", pelota); //Muestro la pelota recibida
                pelota++;
                alarm(1); //Configuro la alarma para 1 segundo
                pause(); //Espero a que llegue la señal de la alarma
                write(h2p[1], &pelota,cont); //Envio la pelota de vuelta al padre                 
            }
            close(p2h[0]);
            close(h2p[1]);
            exit(EXIT_SUCCESS);
        default: //Padre
            close(p2h[0]); //Cierro el extremo de lectura del padre al hijo
            close(h2p[1]); //Cierro el extremo de escritura del hijo al
            cont = read(h2p[0], &pelota, sizeof(pelota)); //Leo la pelota enviada por el padre
            write(p2h[1], &pelota,1 ); //Envio la pelota de vuelta al padre
            while(pelota < 100){
                cont = read(h2p[0], &pelota, sizeof(pelota)); //Leo la pelota enviada por el padre
                printf("Padre recibe: %d\n", pelota); //Muestro la pelota recibida
                pelota++;
                alarm(1); //Configuro la alarma para 1 segundo
                pause(); //Espero a que llegue la señal de la alarma
                write(p2h[1], &pelota,cont ); //Envio la pelota de vuelta al padre                 
            }
            close(p2h[1]);
            close(h2p[0]);
            wait(NULL); //Espero a que el hijo termine


    }
    return 0;
}

