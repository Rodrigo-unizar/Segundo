//Este programa es un ejemplo de como seria el programa real ya que para esto necesitariamos p1 y p2
//Para probar el funcionamiento se recomienda poner cualquier comando como p1 y cat como p2
//Con esto imprimes las n primeras lineas de el comando p1 por la terminal
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    if(argc =! 3){
        write(2, "Numero de parametrso incorrectos" , 33);
        exit(1);
    }

    int fichsal = open(argv[2], 1 | O_CREAT | O_TRUNC, 0666);
    int n = atoi(argv[1]);

    int lineas_leidas = 0;

    char c;

    while (lineas_leidas < n && read(0, &c, 1) > 0) {

        write(1, &c, 1);         // imprimir por pantalla
        write(fichsal, &c, 1);   // imprimir en fichero

        if (c == '\n') {
            lineas_leidas++;
        }
    }


}
