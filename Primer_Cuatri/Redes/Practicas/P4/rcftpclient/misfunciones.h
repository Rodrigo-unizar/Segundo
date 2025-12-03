/****************************************************************************/
/* Plantilla para cabeceras de funciones del cliente (rcftpclient)          */
/* Plantilla $Revision$ */
/* Autor: Apellidos, Nombre */
/* Autor: Apellidos, Nombre (segundo autor, si existe) */
#warning HAY QUE PONER EL NOMBRE (Y BORRAR EL WARNING)
/****************************************************************************/

/**
 * Obtiene la estructura de direcciones del servidor
 *
 * @param[in] dir_servidor String con la dirección de destino
 * @param[in] servicio String con el servicio/número de puerto
 * @param[in] f_verbose Flag para imprimir información adicional
 * @return Dirección de estructura con la dirección del servidor
 */
 /* Flags del programa */
/** @{ */
#define F_VERBOSE	0x1	/**< Flag para mostrar detalles en la salida estándar */
// a bailar!
#define F_SALSA		0x2	/**< Flag para generar respuestas incorrectas y descartando mensajes recibidos */
#define F_FUNKY		0x4	/**< F_SALSA + puede no responder o duplicar mensaje */
#define F_ROCKNROLL	0x8 /**< F_FUNKY + cualquier error, con/sin descartar mensajes recibidos */
/** @} */
/* defines para la salida del programa */
/** @{ */
#define S_OK 0 /**< Flag de salida correcta */
#define S_ABORT 1 /**< Flag de salida incorrecta */
#define S_SYSERROR 2 /**< Flag de salida por error de sistema */
#define S_PROGERROR 3 /**< Flag de salida por error de programa (BUG!) */
#define S_CLIERROR 4 /**< Flag de salida para avisar de error en cliente */
/** @} */
/** @{ */
#define ANSI_COLOR_RED     "\x1b[31m" /**< Pone terminal a rojo */
#define ANSI_COLOR_GREEN   "\x1b[32m" /**< Pone terminal a verde */
#define ANSI_COLOR_YELLOW  "\x1b[33m" /**< Pone terminal a amarillo */
#define ANSI_COLOR_BLUE    "\x1b[34m" /**< Pone terminal a azul */
#define ANSI_COLOR_MAGENTA "\x1b[35m" /**< Pone terminal a magenta */
#define ANSI_COLOR_CYAN    "\x1b[36m" /**< Pone terminal a turquesa */
#define ANSI_COLOR_RESET   "\x1b[0m" /**< Desactiva color del terminal */
struct addrinfo* obtener_struct_direccion(char *dir_servidor, char *servicio, char f_verbose);

/**
 * Imprime una estructura sockaddr_in o sockaddr_in6 almacenada en sockaddr_storage
 *
 * @param[in] saddr Estructura de dirección
 */
void printsockaddr(struct sockaddr_storage * saddr);

/**
 * Configura el socket
 *
 * @param[in] servinfo Estructura con la dirección del servidor
 * @param[in] f_verbose Flag para imprimir información adicional
 * @return Descriptor del socket creado
 */
int initsocket(struct addrinfo *servinfo, char f_verbose);


/**
 * Algoritmo 1 del cliente
 *
 * @param[in] socket Descriptor del socket
 * @param[in] servinfo Estructura con la dirección del servidor
 */
void alg_basico(int socket, struct addrinfo *servinfo);


/**
 * Algoritmo 2 del cliente
 *
 * @param[in] socket Descriptor del socket
 * @param[in] servinfo Estructura con la dirección del servidor
 */
void alg_stopwait(int socket, struct addrinfo *servinfo);


/**
 * Algoritmo 3 del cliente
 *
 * @param[in] socket Descriptor del socket
 * @param[in] servinfo Estructura con la dirección del servidor
 * @param[in] window Tamaño deseado de la ventana deslizante
 */
void alg_ventana(int socket, struct addrinfo *servinfo,int window);


