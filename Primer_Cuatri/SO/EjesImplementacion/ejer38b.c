#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

// Estructura para pasar parámetros a los hilos
typedef struct {
    int* data;          // Puntero a los datos mapeados
    int start;          // Índice de inicio
    int count;          // Número de elementos a procesar
    long partial_sum;   // Suma parcial calculada
} thread_data_t;

// Función que ejecuta cada hilo
void* thread_function(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    long sum = 0;
    
    for (int i = 0; i < data->count; i++) {
        sum += data->data[data->start + i];
    }
    
    data->partial_sum = sum;
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        write(1, "Numero de parametros incorrectos\n", 33);
        exit(0);
    }

    int n = atoi(argv[2]);
    
    // Abrir el archivo
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
    
    // Obtener el tamaño del archivo
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Error al obtener información del archivo");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    long fsize = sb.st_size;
    int total = fsize / sizeof(int);
    
    // Mapear el archivo en memoria
    int* mapped_data = mmap(NULL, fsize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_data == MAP_FAILED) {
        perror("Error al mapear el archivo");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    // No necesitamos mantener el descriptor abierto después del mapeo
    close(fd);
    
    // Calcular el tamaño de cada tramo
    int tramo = total / n;
    
    // Crear arreglos para hilos y datos
    pthread_t threads[n];
    thread_data_t thread_data[n];
    
    // Crear y lanzar los hilos
    for (int i = 0; i < n; i++) {
        thread_data[i].data = mapped_data;
        thread_data[i].start = i * tramo;
        thread_data[i].count = tramo;
        thread_data[i].partial_sum = 0;
        
        if (pthread_create(&threads[i], NULL, thread_function, &thread_data[i]) != 0) {
            perror("Error al crear el hilo");
            exit(EXIT_FAILURE);
        }
    }
    
    long resFin = 0;
    
    // Esperar a que todos los hilos terminen y recoger resultados
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
        resFin += thread_data[i].partial_sum;
    }
    
    // Procesar el resto (si lo hay) en el hilo principal
    int resto = total - tramo * n;
    if (resto > 0) {
        for (int i = 0; i < resto; i++) {
            resFin += mapped_data[tramo * n + i];
        }
    }
    
    // Desmapear la memoria
    munmap(mapped_data, fsize);
    
    // Mostrar el resultado
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "%ld", resFin);
    write(1, "El numero final es: ", 20);
    write(1, buf, len);
    write(1, "\n", 1);
    
    return 0;
}