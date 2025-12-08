/****************************************************************************/
/* Plantilla para implementación de funciones del cliente (rcftpclient)     */
/* $Revision$ */
/* Aunque se permite la modificación de cualquier parte del código, se */
/* recomienda modificar solamente este fichero y su fichero de cabeceras asociado. */
/****************************************************************************/

/**************************************************************************/
/* INCLUDES                                                               */
/**************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include "rcftp.h" // Protocolo RCFTP
#include "rcftpclient.h" // Funciones ya implementadas
#include "multialarm.h" // Gestión de timeouts
#include "vemision.h" // Gestión de ventana de emisión
#include "misfunciones.h"

/**************************************************************************/
/* VARIABLES GLOBALES                                                     */
/**************************************************************************/

// elegir 1 o 2 autores y sustituir "Apellidos, Nombre" manteniendo el formato
//char* autores="Autor: Apellidos, Nombre"; // un solo autor
char* autores="Autor: Ghita Calangiu, Razvan\nAutor: Herce Alonso, Rodrigo";	 // dos autores

// variable para indicar si mostrar información extra durante la ejecución
// como la mayoría de las funciones necesitaran consultarla, la definimos global
extern char verb;


// variable externa que muestra el número de timeouts vencidos
// Uso: Comparar con otra variable inicializada a 0; si son distintas, tratar un timeout e incrementar en uno la otra variable
extern volatile const int timeouts_vencidos;


/**************************************************************************/
/* Obtiene la estructura de direcciones del servidor */
/**************************************************************************/
struct addrinfo* obtener_struct_direccion(char *dir_servidor, char *servicio, char f_verbose)
{
    struct addrinfo hints,     // Variable para especificar la solicitud
                    *servinfo, // Puntero para respuesta de getaddrinfo()
                    *direccion;// Puntero para recorrer la lista de
                               // direcciones de servinfo
    int status;     // Finalización correcta o no de la llamada getaddrinfo()
    int numdir = 1; // Contador de estructuras de direcciones en la
                    // lista de direcciones de servinfo

    // sobreescribimos con ceros la estructura
    // para borrar cualquier dato que pueda malinterpretarse
    memset(&hints, 0, sizeof hints);

    // genera una estructura de dirección con especificaciones de la solicitud
    if (f_verbose)
    {
        printf("1 - Especificando detalles de la estructura de direcciones a solicitar... \n");
        fflush(stdout);
    }

    // especificamos la familia de direcciones con la que queremos trabajar:
    // AF_UNSPEC, AF_INET (IPv4), AF_INET6 (IPv6), etc.
    hints.ai_family = AF_UNSPEC;

    if (f_verbose)
    {
        printf("\tFamilia de direcciones/protocolos: ");
        switch (hints.ai_family)
        {
            case AF_UNSPEC: printf("IPv4 e IPv6\n"); break;
            case AF_INET:   printf("IPv4\n"); break;
            case AF_INET6:  printf("IPv6\n"); break;
            default:        printf("No IP (%d)\n", hints.ai_family); break;
        }
        fflush(stdout);
    }

    // especificamos el tipo de socket deseado:
    // SOCK_STREAM (TCP), SOCK_DGRAM (UDP), etc.
    hints.ai_socktype = SOCK_DGRAM;

    if (f_verbose)
    {
        printf("\tTipo de comunicación: ");
        switch (hints.ai_socktype)
        {
            case SOCK_STREAM: printf("flujo (TCP)\n"); break;
            case SOCK_DGRAM:  printf("datagrama (UDP)\n"); break;
            default:          printf("no convencional (%d)\n", hints.ai_socktype); break;
        }
        fflush(stdout);
    }

    // flags específicos dependiendo de si queremos la dirección como cliente
    // o como servidor
    if (dir_servidor != NULL)
    {
        // si hemos especificado dir_servidor, es que somos el cliente
        // y vamos a conectarnos con dir_servidor
        if (f_verbose) printf("\tNombre/dirección del equipo: %s\n", dir_servidor);
    }
    else
    {
        // si no hemos especificado, es que vamos a ser el servidor
        if (f_verbose) printf("\tNombre/dirección: equipo local\n");

        // especificar flag para que la IP se rellene con lo necesario para hacer bind
        // consultar documentación con: 'man getaddrinfo')
        hints.ai_flags = AI_PASSIVE;
    }
    if (f_verbose) printf("\tServicio/puerto: %s\n", servicio);

    // llamada getaddrinfo() para obtener la estructura de direcciones solicitada
    // getaddrinfo() pide memoria dinámica al SO,
    // la rellena con la estructura de direcciones
    // y escribe en servinfo la dirección donde se encuentra dicha estructura.
    // La memoria dinámica reservada en una función NO se libera al salir de ella
    // Para liberar esta memoria, usar freeaddrinfo()
    if (f_verbose)
    {
        printf("2 - Solicitando la estructura de direcciones con getaddrinfo()... ");
        fflush(stdout);
    }
    status = getaddrinfo(dir_servidor, servicio, &hints, &servinfo);
    if (status != 0)
    {
        fprintf(stderr,"Error en la llamada getaddrinfo(): %s\n", gai_strerror(status));
        exit(1);
    }
    if (f_verbose) printf("hecho\n");

    // imprime la estructura de direcciones devuelta por getaddrinfo()
    if (f_verbose)
    {
        printf("3 - Analizando estructura de direcciones devuelta... \n");
        direccion = servinfo;
        while (direccion != NULL)
        {   // bucle que recorre la lista de direcciones
            printf("    Dirección %d:\n", numdir);
            printsockaddr((struct sockaddr_storage*) direccion->ai_addr);
            // "avanzamos" a la siguiente estructura de direccion
            direccion = direccion->ai_next;
            numdir++;
        }
    }

    // devuelve la estructura de direcciones devuelta por getaddrinfo()
    return servinfo;
}
/**************************************************************************/
/* Imprime una direccion */
/**************************************************************************/
void printsockaddr(struct sockaddr_storage * saddr)
{
    struct sockaddr_in *saddr_ipv4; // puntero a estructura de dirección IPv4
    // el compilador interpretará lo apuntado como estructura de dirección IPv4
    struct sockaddr_in6 *saddr_ipv6; // puntero a estructura de dirección IPv6
    // el compilador interpretará lo apuntado como estructura de dirección IPv6
    void *addr; // puntero a dirección
    // como puede ser tipo IPv4 o IPv6 no queremos que el compilador la
    // interprete de alguna forma particular, por eso void
    char ipstr[INET6_ADDRSTRLEN]; // string para la dirección en formato texto
    int port; // almacena el número de puerto al analizar estructura devuelta

    if (saddr == NULL)
    {
        printf("La dirección está vacía\n");
    }
    else
    {
        printf("\tFamilia de direcciones: ");
        fflush(stdout);
        if (saddr->ss_family == AF_INET6)
        {   // IPv6
            printf("IPv6\n");
            // apuntamos a la estructura con saddr_ipv6 (cast evita warning),
            // así podemos acceder al resto de campos a través de
            // este puntero sin más casts
            saddr_ipv6 = (struct sockaddr_in6 *)saddr;
            // apuntamos al campo de la estructura que contiene la dirección
            addr = &(saddr_ipv6->sin6_addr);
            // obtenemos el puerto, pasando del formato de red al formato local
            port = ntohs(saddr_ipv6->sin6_port);
        }
        else if (saddr->ss_family == AF_INET)
        {   // IPv4
            printf("IPv4\n");
            saddr_ipv4 = (struct sockaddr_in*)saddr ;
            addr = &(saddr_ipv4->sin_addr);
            port = ntohs(saddr_ipv4->sin_port);
        }
        else
        {
            fprintf(stderr, "familia desconocida\n");
            exit(1);
        }
        // convierte la dirección ip a string
        inet_ntop(saddr->ss_family, addr, ipstr, sizeof ipstr);
        printf("\tDirección (interpretada según familia): %s\n", ipstr);
        printf("\tPuerto (formato local): %d\n", port);
    }
}
/**************************************************************************/
/* Configura el socket, devuelve el socket y servinfo */
/**************************************************************************/
int initsocket(struct addrinfo *servinfo, char f_verbose){
    int sock = -1;

    // crea un extremo de la comunicación y devuelve un descriptor
    if(f_verbose){
    	printf("Creando el socket (socket)... ");
        fflush(stdout);
    }

    sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if(sock < 0){
    	perror("Error en la llamada socket: No se pudo crear el socket");
        // muestra por pantalla el valor de la cadena suministrada por el
        // programador, dos puntos y un mensaje de error que detalla la
        // causa del error cometido

    } else  {   // socket creado correctamente
        if (f_verbose) printf("hecho\n");
    }

    if (sock < 0)
    {
        perror("No se ha podido establecer la comunicación");
        exit(1);
    }

    return sock;
}


void construirPMensajeRCFTP(struct rcftp_msg *sendbuffer, int datos) {
    // rellenar sendbuffer proporcionado
    sendbuffer->version = RCFTP_VERSION_1;
    sendbuffer->numseq = htonl(0);
    sendbuffer->len = htons(datos);
    sendbuffer->next = htonl(0);
    sendbuffer->flags = 0;
    sendbuffer->sum = 0;
    // calcular checksum sobre toda la estructura
    sendbuffer->sum = xsum((char*)sendbuffer, sizeof(*sendbuffer));
}


void enviamensaje(int s, struct rcftp_msg sendbuffer, struct sockaddr_storage remote, socklen_t remotelen) {
	ssize_t sentsize;

	if ((sentsize=sendto(s,(char *)&sendbuffer,sizeof(sendbuffer),0,(struct sockaddr *)&remote,remotelen)) != sizeof(sendbuffer)) {
		if (sentsize!=-1)
			fprintf(stderr,"Error: enviados %d bytes de un mensaje de %d bytes\n",(int)sentsize,(int)sizeof(sendbuffer));
		else
			perror("Error en sendto");
		exit(S_SYSERROR);
	} 

	// print response if in verbose mode
	if (verb) {
		printf("Mensaje RCFTP " ANSI_COLOR_MAGENTA "enviado" ANSI_COLOR_RESET ":\n");
		print_rcftp_msg(&sendbuffer,sizeof(sendbuffer));
	} 
}	


ssize_t recibirmensaje(int socket, struct rcftp_msg *buffer, int buflen, struct sockaddr_storage *remote, socklen_t *remotelen) {
	ssize_t recvsize;
	
	*remotelen = sizeof(*remote);
	recvsize = recvfrom(socket,(char *)buffer,buflen,0,(struct sockaddr *)remote,remotelen);
	if (recvsize<0 && errno!=EAGAIN) { // en caso de socket no bloqueante
		//if (recvsize<0 && errno!=EINTR) { // en caso de socket bloqueante (no funciona en GNU/Linux)
		perror("Error en recvfrom: ");
		exit(S_SYSERROR);
	} else if (*remotelen>sizeof(*remote)) {
		fprintf(stderr,"Error: la dirección del cliente ha sido truncada\n");
		exit(S_SYSERROR);
	}

	return recvsize;
}

int esMensajeValido(struct rcftp_msg msgRecibido) {
    int esperado=1;
    //uint16_t aux;

    if (msgRecibido.version!=RCFTP_VERSION_1) { // versión incorrecta
        esperado=0;
        fprintf(stderr,"Error: recibido un mensaje con versión incorrecta\n");
    }
    /*if (msgRecibido.next!=0) { // next incorrecto
        esperado=0;
        fprintf(stderr,"Error: recibido un mensaje con NEXT incorrecto\n");
    }*/
    if (issumvalid(&msgRecibido,sizeof(msgRecibido))==0) { // checksum incorrecto
        esperado=0;
        fprintf(stderr,"Error: recibido un mensaje con checksum incorrecto\n"); /* (esperaba ");
        aux=recvbuffer.sum;
        recvbuffer.sum=0;
        fprintf(stderr,"0x%x)\n",ntohs(xsum((char*)&recvbuffer,sizeof(recvbuffer))));
        recvbuffer.sum=aux;*/
    }
    return esperado;
}

int esMensajeEsperado(struct rcftp_msg msgRecibido, struct rcftp_msg mensajeEsperado){
    if(ntohl(msgRecibido.next) != (ntohl(mensajeEsperado.numseq) + ntohs(mensajeEsperado.len))){
        return 0;
    }else if(msgRecibido.flags & F_BUSY || msgRecibido.flags & F_ABORT){
        return 0;
    }else if(mensajeEsperado.flags & F_FIN && !(msgRecibido.flags & F_FIN)){
        return 0;
    }
    return 1;
}


/**************************************************************************/
/*  algoritmo 1 (basico)  */
/**************************************************************************/
void alg_basico(int socket, struct addrinfo *servinfo) {

	printf("Comunicación con algoritmo básico\n");

	bool ultimoMensaje = false;
	bool ultimoMensajeConfirmado = false;
	struct rcftp_msg mensaje;
    struct rcftp_msg msgRecibido;
	struct sockaddr_storage remote;
    socklen_t remotelen;

    memcpy(&remote, servinfo->ai_addr, servinfo->ai_addrlen);
    remotelen = servinfo->ai_addrlen;

	int datos = readtobuffer((char *)mensaje.buffer, RCFTP_BUFLEN);

	if(datos == 0) {
		ultimoMensaje = true;
	}
	
	construirPMensajeRCFTP(&mensaje, datos);

	while(!ultimoMensajeConfirmado){
		enviamensaje(socket, mensaje,remote,remotelen);
        if(recibirmensaje(socket, &msgRecibido, sizeof(msgRecibido), &remote, &remotelen) > 0){
            if(esMensajeValido(msgRecibido) && esMensajeEsperado(msgRecibido, mensaje)){
                if(ultimoMensaje){
                    ultimoMensajeConfirmado = true;
                } else {
                    datos = readtobuffer((char *)mensaje.buffer, RCFTP_BUFLEN);

                    if(datos <= 0) {
                        ultimoMensaje = true;
                        mensaje.flags = F_FIN;
                    }

                if(!ultimoMensaje){
                    mensaje.flags = 0;  //no tiene flags
                }
                    //Construimos el siguiente mensaje
                    mensaje.version = RCFTP_VERSION_1;
                    mensaje.numseq = msgRecibido.next;
                    mensaje.len = htons(datos);
                    mensaje.next = htonl(0);
                    mensaje.sum = 0;
                    // calcular checksum sobre toda la estructura
                    mensaje.sum = xsum((char*)&mensaje, sizeof(mensaje));
            
                }
            }
	    }
    }
}

ssize_t recibirmensajeSW(int socket, struct rcftp_msg *buffer, int buflen, struct sockaddr_storage *remote, socklen_t *remotelen, int *timeouts_procesados) {
	
    bool esperar =true;
    ssize_t recvsize;
    *remotelen = sizeof(*remote);

    while(esperar){
      
        recvsize = recvfrom(socket,(char *)buffer,buflen,0,(struct sockaddr *)remote,remotelen);
        if (recvsize<0 && errno!=EAGAIN) { // en caso de socket no bloqueante
            //if (recvsize<0 && errno!=EINTR) { // en caso de socket bloqueante (no funciona en GNU/Linux)
            perror("Error en recvfrom: ");
            exit(S_SYSERROR);
        } else if (*remotelen>sizeof(*remote)) {
            fprintf(stderr,"Error: la dirección del cliente ha sido truncada\n");
            exit(S_SYSERROR);
        }else if(recvsize>0){
            canceltimeout();
            esperar=false;
        }
        if (*timeouts_procesados != timeouts_vencidos){
            esperar = false;
            (*timeouts_procesados)++;
        }
        
    }
    
	return recvsize;
}

void enviamensajeSW(int s, struct rcftp_msg sendbuffer, struct sockaddr_storage remote, socklen_t remotelen) {
	ssize_t sentsize;

	if ((sentsize=sendto(s,(char *)&sendbuffer,sizeof(sendbuffer),0,(struct sockaddr *)&remote,remotelen)) != sizeof(sendbuffer)) {
		if (sentsize!=-1)
			fprintf(stderr,"Error: enviados %d bytes de un mensaje de %d bytes\n",(int)sentsize,(int)sizeof(sendbuffer));
		else
			perror("Error en sendto");
		exit(S_SYSERROR);
	} 

	// print response if in verbose mode
	if (verb) {
		printf("Mensaje RCFTP " ANSI_COLOR_MAGENTA "enviado" ANSI_COLOR_RESET ":\n");
		print_rcftp_msg(&sendbuffer,sizeof(sendbuffer));
	} 
    
}	

/**************************************************************************/
/*  algoritmo 2 (stop & wait)  */
/**************************************************************************/

void alg_stopwait(int socket, struct addrinfo *servinfo) {

	printf("Comunicación con algoritmo stop&wait\n");

    signal(SIGALRM, handle_sigalrm); // definir manejador de la señal de alarma
    int sockflags = fcntl(socket, F_GETFL, 0); //obtiene valor de los flags 
    fcntl(socket, F_SETFL, sockflags | O_NONBLOCK); //modifica el flag de bloqueo
    int timeouts_procesados = 0;

    bool ultimoMensaje = false;
	bool ultimoMensajeConfirmado = false;
	struct rcftp_msg mensaje;
    struct rcftp_msg msgRecibido;
	struct sockaddr_storage remote;
    socklen_t remotelen;

    memcpy(&remote, servinfo->ai_addr, servinfo->ai_addrlen);
    remotelen = servinfo->ai_addrlen;

	int datos = readtobuffer((char *)mensaje.buffer, RCFTP_BUFLEN);

	if(datos == 0) {
		ultimoMensaje = true;
	}
	
	construirPMensajeRCFTP(&mensaje, datos);

	while(!ultimoMensajeConfirmado){
		enviamensajeSW(socket, mensaje,remote,remotelen);
        if(addtimeout()){
            if(recibirmensajeSW(socket, &msgRecibido, sizeof(msgRecibido), &remote, &remotelen, &timeouts_procesados) > 0){
                if(esMensajeValido(msgRecibido) && esMensajeEsperado(msgRecibido, mensaje)){
                    if(ultimoMensaje){
                        ultimoMensajeConfirmado = true;
                    } else {
                        datos = readtobuffer((char *)mensaje.buffer, RCFTP_BUFLEN);

                        if(datos <= 0) {
                            ultimoMensaje = true;
                            mensaje.flags = F_FIN;
                        }

                    if(!ultimoMensaje){
                        mensaje.flags = 0;  //no tiene flags
                    }
                        //Construimos el siguiente mensaje
                        mensaje.version = RCFTP_VERSION_1;
                        mensaje.numseq = msgRecibido.next;
                        mensaje.len = htons(datos);
                        mensaje.next = htonl(0);
                        mensaje.sum = 0;
                        // calcular checksum sobre toda la estructura
                        mensaje.sum = xsum((char*)&mensaje, sizeof(mensaje));
                
                    }
                }
            }
        }       
        
    }
}

/*
int esRespuestaEsperadaGBN(struct rcftp_msg respuesta, struct rcftp_msg mensaje, int tventana){
    uint32_t ack = ntohl(respuesta.next);
    
    // El ACK debe estar entre la base de la ventana y el siguiente a enviar
    if(ack < base_ventana || ack > siguiente_seq){
        return 0;
    }
    
    if(respuesta.flags & F_BUSY || respuesta.flags & F_ABORT){
        return 0;
    }
    
    return 1;
}
*/
void contruirMensajeMasViejoVentana(struct rcftp_msg *mensaje, int *longitudVentana, struct rcftp_msg msgRecibido) {
    int datos = *longitudVentana;
    uint32_t ns = getdatatoresend((char *)mensaje->buffer, &datos);
    mensaje->numseq = htonl(ns);
    mensaje->version = RCFTP_VERSION_1;
    mensaje->len = htons(datos);
    mensaje->next = htonl(0);
    mensaje->sum = 0;
    // calcular checksum sobre toda la estructura
    mensaje->sum = xsum((char*)mensaje, sizeof(*mensaje));
    *longitudVentana = datos;
}

ssize_t recibirmensajeGBN(int socket, struct rcftp_msg *buffer, int buflen, struct sockaddr_storage *remote, socklen_t *remotelen, int *timeouts_procesados) {
	
    bool esperar =true;
    ssize_t recvsize;
    *remotelen = sizeof(*remote);

    while(esperar){
      
        recvsize = recvfrom(socket,(char *)buffer,buflen,0,(struct sockaddr *)remote,remotelen);
        if (recvsize<0 && errno!=EAGAIN) { // en caso de socket no bloqueante
            //if (recvsize<0 && errno!=EINTR) { // en caso de socket bloqueante (no funciona en GNU/Linux)
            perror("Error en recvfrom: ");
            exit(S_SYSERROR);
        } else if (*remotelen>sizeof(*remote)) {
            fprintf(stderr,"Error: la dirección del cliente ha sido truncada\n");
            exit(S_SYSERROR);
        }else if(recvsize>0){
            
            esperar=false;
        }
        if (*timeouts_procesados != timeouts_vencidos){
            esperar = false;
            (*timeouts_procesados)++;
        }
        
    }
    
	return recvsize;
}


/**************************************************************************/
/*  algoritmo 3 (ventana deslizante)  */
/**************************************************************************/
void alg_ventana(int socket, struct addrinfo *servinfo,int window) {

	printf("Comunicación con algoritmo go-back-n\n");

    signal(SIGALRM, handle_sigalrm); // definir manejador de la señal de alarma
    int sockflags = fcntl(socket, F_GETFL, 0); //obtiene valor de los flags 
    fcntl(socket, F_SETFL, sockflags | O_NONBLOCK); //modifica el flag de bloqueo
    int timeouts_procesados = 0;
    int datosVentana = 0;
    int datos = 0;

    setwindowsize(window);

    bool ultimoMensaje = false;
	bool ultimoMensajeConfirmado = false;
    bool timeout_act = false;
	struct rcftp_msg mensaje;
    struct rcftp_msg msgRecibido;
	struct sockaddr_storage remote;
    socklen_t remotelen;
    uint32_t siguiente_numseq = 0;  // Siguiente número de secuencia a enviar

    memcpy(&remote, servinfo->ai_addr, servinfo->ai_addrlen);
    remotelen = servinfo->ai_addrlen;

    msgRecibido.next = htonl(0);

    while(!ultimoMensajeConfirmado){
        /*  BLOQUE DE ENVÍO  */
        while((getfreespace() >= RCFTP_BUFLEN) && !ultimoMensaje){
            datos = readtobuffer((char *)mensaje.buffer, RCFTP_BUFLEN);
            uint32_t numseq_actual = siguiente_numseq;
            if(datos == 0){
                ultimoMensaje = true;
                mensaje.flags = F_FIN;
            }
            siguiente_numseq += datos;

            mensaje.version = RCFTP_VERSION_1;
            mensaje.numseq = htonl(numseq_actual);
            mensaje.len = htons(datos);
            mensaje.next = htonl(0);
            mensaje.sum = 0;
            // calcular checksum sobre toda la estructura
            mensaje.sum = xsum((char*)&mensaje, sizeof(mensaje));

            enviamensajeSW(socket,mensaje,remote,remotelen);
            
            if((datosVentana = addsentdatatowindow((char *)mensaje.buffer, datos)) != datos){
                fprintf(stderr, "Se han intentado añadir %d datos a la ventana, pero se han añadido %d datos.\n", datos, datosVentana);
            }

            
            addtimeout();
               
        }

        /*  BLOQUE DE RECEPCIÓN  */
        ssize_t recvsize = recibirmensajeGBN(socket, &msgRecibido, sizeof(msgRecibido), 
                                    &remote, &remotelen, &timeouts_procesados);

        if(recvsize > 0){
            
            if(esMensajeValido(msgRecibido)){
                // Verificar que no sea BUSY o ABORT
                if(!(msgRecibido.flags & F_BUSY) && !(msgRecibido.flags & F_ABORT)){
                    uint32_t ack = ntohl(msgRecibido.next);
                    
                    // El ACK debe ser mayor que 0 y no mayor que lo que hemos enviado
                    if(ack > 0 && ack <= siguiente_numseq){
                        uint32_t base = getnumseqfirst();

                        if (ack > base && ack <= siguiente_numseq) {
                            if (timeout_act) {
                                
                                timeout_act = false;
                            }

                            freewindow(ack);
                        }
                        
                        // Si recibimos ACK del último mensaje con FIN
                        if((msgRecibido.flags & F_FIN) && ultimoMensaje){
                            ultimoMensajeConfirmado = true;
                        }
                    }
                }
            }else{
                printf("DEBUG: recibido mensaje inválido, esperando timeout para reenviar\n");
            }
        }       

        /*  BLOQUE DE PROCESADO DE TIMEOUT  */
        if(timeouts_procesados != timeouts_vencidos){
            printf("DEBUG: timeouts_procesados=%d, timeouts_vencidos=%d\n", 
                timeouts_procesados, timeouts_vencidos);
            datosVentana = RCFTP_BUFLEN;
            contruirMensajeMasViejoVentana(&mensaje, &datosVentana, msgRecibido);
            enviamensajeSW(socket,mensaje,remote,remotelen);
            addtimeout();
            timeouts_procesados++;
            timeout_act = true;
        }
    }
}


