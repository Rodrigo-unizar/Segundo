#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        write(2, "Uso: mover <origen> <destino>\n", 31);
        return 1;
    }

    /* Crear una nueva entrada de directorio que apunte al mismo i-node */
    if (link(argv[1], argv[2]) == -1) {
        perror("link");
        return 1;
    }

    /* Eliminar el nombre antiguo */
    if (unlink(argv[1]) == -1) {
        perror("unlink");
        return 1;
    }

    return 0;
}