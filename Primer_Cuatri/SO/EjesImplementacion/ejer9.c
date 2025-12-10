//Este ejercicio no puede ser compilado ya que no disponemos de las funciones leer_sensor y actua
//No obstante aqui esta como seria la implementación del ejercicio. 

void fcaptura(){
    siganl(14, fcaptura)
}

int main(){
    signal(14, fcaptura);
    int fd[2];
    pipe(fd);
    switch(fork()){
        case -1:
            fprintf(stderr, "Error en el fork");
            exit(1);
        case 0: //actuador
            int fdat = open("datos.dat", 1 | O_CREAT | O_TRUNC, 0666);
            int dato;
            ssize_t readBytes;
            close(fd[1]);
            while((readBytes = read(fd[0], &dato, sizeof(int) - 1)) > 0){
                actua(dato);
                write(fdat,buffer,readBytes );
            }
            close(fd[0]);
            write(1, "Terminando actuador",19 );
            exit(0)
        default:
            int dato;
            close(fd[0]);
            ssize_t sendBytes;
            alarm(1);
            pause();
            dato = leer_sensor();
            while((sendBytes = write(fd[1],&dato, sizeof(int))) > 0){
                alarm(1);
                pause();
                dato = leer_sensor();
            }
            close(fd[1]);
            wait(NULL);
            write(1, "Terminando sensor", 17);
            exit(0);

    }

}