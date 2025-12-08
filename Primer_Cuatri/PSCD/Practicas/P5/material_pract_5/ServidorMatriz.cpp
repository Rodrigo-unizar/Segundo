//******************************************************************
// File:   ServidorMulticliente.cpp
// Author: PSCD-Unizar
// Date:   Noviembre 2022
// Coms:   Ejemplo de servidor multicliente con comunicación síncrona mediante sockets
//         Invocación
//                  ./ServidorMulticliente
//*****************************************************************

#include "Socket.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include "monitorMatriz.hpp"

using namespace std;



//-------------------------------------------------------------
// Espera "secs" segundos y se conecta. Usado para desbloquear un "accept"
// Y pone "fin" a true

//-------------------------------------------------------------
void servCliente(Socket& chan, int client_fd, monitor& m, int id) {
    
    // Buffer para recibir el mensaje
    int length = 100;
    string buffer;
    const string msgFin = "END";
    string tarFin = "TF";


    bool out = false; // Inicialmente no salir del bucle
    while(!out) {
        tarea T;
        int i = 0;
        // Recibimos el mensaje del cliente
        int rcv_bytes = chan.Receive(client_fd,buffer,length);

        if (rcv_bytes == -1) {
            cerr << chan.error("Error al recibir datos");
            // Cerramos los sockets
            chan.Close(client_fd);
        }

       while(buffer[i] != ',') {
            T.tipoTarea += buffer[i];
            i++;
        }

        // Si recibimos "END OF SERVICE" --> Fin de la comunicación
        if (tarFin == T.tipoTarea) {
            out = true; // Salir del bucle
        } else { 
            bool exito = true;
            
            int j = 0;
            i++; //saltamos la coma
            while (buffer[i] != ','){
                j++;
                i++;
            }
            i++; //saltamos la coma
            if(j != 2){
                exito = false;
            }
            string cargaStr = "";
            while (i < buffer.length() && buffer[i] != '\0'){
                cargaStr += buffer[i];
                i++;
            }
            T.cargaDeTrabajo = stof(cargaStr);
            cout << "Servidor: Tarea recibida de tipo " + T.tipoTarea + " con carga de trabajo " + to_string(T.cargaDeTrabajo) + "\n";
            m.escribirMatriz(T, exito);
            // Enviamos la respuesta
            
            string s = "TASK_RECEIVED";
            int send_bytes = chan.Send(client_fd, s);
        }
    }
    int send_bytes = chan.Send(client_fd, msgFin);
    chan.Close();

    cout << "Cliente " + to_string(id) + " finalizado\n";
}
//-------------------------------------------------------------
int main(int argc,char* argv[]) {
    const int N = 4;
    // Puerto donde escucha el proceso servidor
    int SERVER_PORT = (stoi(argv[1]) + 413); //normalmente será un parámetro de invocación. P.e.: argv[1]
    vector<thread> cliente;
    MultiBuffer<tarea,N_CONTROLLERS> mBT;

    monitor m;


    bool fin = false;

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_PORT);

    // bind
    int socket_fd = chan.Bind();
    if (socket_fd == -1) {
        cerr << chan.error("Error en el bind");
        exit(1);
    }

    //lisen
    int error_code = chan.Listen();
    if (error_code == -1) {
        cerr << chan.error("Error en el listen");
        // Cerramos el socket
        chan.Close();
        exit(1);
    }
    //para desbloquear servidor y terminar
    int j = 0;
    int i = 0;
    while (!fin) {
    	i++;
        // Accept
        int new_client_fds = chan.Accept();

        if(new_client_fds == -1) {
            cerr << chan.error("Error en el accept");
            // Cerramos el socket
            chan.Close();
            exit(1);
        } else {
        	if (!fin) {
                //introducir en el vector el cliente y arrancar el thread
        	    cliente.push_back(thread(&servCliente, ref(chan), new_client_fds, ref(m), j));
        	    cout << "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
                j++;
        	}
        	else {
        		cout << "He acabado" << endl;
        	}
        }
    }
    cout << "Servidor matriz finalizando. Esperando a los clientes...\n";
    //¿Qué pasa si algún thread acaba inesperadamente?
    for (int i=0; i<cliente.size(); i++) {
        cliente[i].join();
    }
    cout << "Servidor matriz finalizando. Matriz final:\n";
    m.mostrarMatriz();

    // Cerramos el socket del servidor
    error_code = chan.Close();
    if (error_code == -1) {
        cerr << chan.error("Error cerrando el socket del servidor");
    }

    // Despedida
    cout << "Bye bye" << endl;

    return error_code;
}
//-------------------------------------------------------------
