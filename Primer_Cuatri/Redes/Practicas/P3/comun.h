/**
 * @file comun.h
 *
 * Este fichero contiene cabeceras comunes para trabajar con 
 * estructuras de direcciones.
 */

#ifndef COMUN_H
#define COMUN_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/**
 * Imprime una estructura sockaddr_in o sockaddr_in6 almacenada en sockaddr_storage.
 * 
 * @param saddr Puntero a la estructura de dirección a imprimir.
 */
void printsockaddr(struct sockaddr_storage * saddr);

/**
 * Obtiene una estructura de direcciones según los parámetros especificados.
 * 
 * @param dir_servidor String con el nombre/dirección IP del equipo. NULL para equipo local.
 * @param servicio String con el servicio/puerto a solicitar.
 * @param f_verbose Si es distinto de 0, muestra información extra.
 * @return Puntero a la estructura de direcciones (debe liberarse con freeaddrinfo).
 */
struct addrinfo* obtener_struct_direccion(char *dir_servidor, char *servicio, char f_verbose);

#endif /* COMUN_H */
