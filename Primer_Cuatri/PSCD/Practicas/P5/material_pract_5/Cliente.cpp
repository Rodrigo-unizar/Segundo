//*****************************************************************
// File:   Cliente.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2022
// Coms:   Ejemplo de cliente con comunicación síncrona mediante sockets
//         Invocación
//                  ./Cliente
//*****************************************************************
#include <iostream>
#include <chrono>
#include <thread>
#include "Socket.hpp"
#include "monitor.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

void fcliente(int SERVER_PORT, string SERVER_ADDRESS) {
    
    const string MENS_FIN = "END";
    const string atask = "GET_TASK";
    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket Stareas(SERVER_ADDRESS, SERVER_PORT);
    Socket Smatriz(SERVER_ADDRESS, SERVER_PORT + 413); 

    // Conectamos con el servidor. Probamos varias conexiones
    const int MAX_ATTEMPS = 10;
    int count = 0;
    int socket_fd;
    int socket_fd2;
    do {
        // Conexión con el servidor
        socket_fd = Stareas.Connect();
        socket_fd2 = Smatriz.Connect();
        count++;

        // Si error --> esperamos 1 segundo para reconectar
        if(socket_fd == -1 || socket_fd2 == -1) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while((socket_fd == -1 || socket_fd2 == -1) && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_fd == -1 || socket_fd2 == -1) {
        cerr << Stareas.error("No se ha podido conectar con el servidor");
        exit(1);
    }

    
    int read_bytes;   //num de bytes recibidos en un mensaje
    int send_bytes;  //num de bytes enviados en un mensaje
    bool fin = false;
    do {
        // Leer mensaje de la entrada estandar
        
        // Enviamos el mensaje
        send_bytes = Stareas.Send(socket_fd, atask);

        if(send_bytes == -1) {
            cerr << Stareas.error("Error al enviar datos");
            // Cerramos el socket
            Stareas.Close();
            exit(1);
        }

        
        // Buffer para almacenar la respuesta
        string buffer;

        // Recibimos la respuesta del servidor
        read_bytes = Stareas.Receive(socket_fd, buffer, MESSAGE_SIZE);
        
        int i = 0;
        int num_exist = 0;
            bool exito = true;
            string tipoTarea = "";
            string cargaStr = "";
            while(buffer[i] != ',') {
                tipoTarea += buffer[i];
                i++;
            }
            i++; //saltamos la coma
            while (i < buffer.length() && buffer[i] != '\0'){
                i++;
                cargaStr += buffer[i];
            }
            if(tipoTarea == "t1"){
                num_exist =1 + rand() % 100;
                exito = num_exist > 5;
            }else if(tipoTarea == "t2"){
                num_exist =1 + rand() % 100;
                exito = num_exist > 7;
            }else if(tipoTarea == "t3"){
                num_exist =1 + rand() % 100;
                exito = num_exist > 10;
            }
            i++; //saltamos la coma
            if(exito){
                buffer = tipoTarea + "," + "OK," + cargaStr;
            }else{
                buffer = tipoTarea + "," + "NO_OK," + cargaStr;
            }
            

        send_bytes = Smatriz.Send(socket_fd2, buffer);
        if(send_bytes == -1) {
            cerr << Smatriz.error("Error al enviar datos");
            // Cerramos el socket
            Smatriz.Close();
            exit(1);
        }
        // Recibimos la respuesta del servidor
        read_bytes = Smatriz.Receive(socket_fd2, buffer, MESSAGE_SIZE);
        cout << "Respuesta del servidor de matriz: " + buffer + "\n";
        if(buffer == MENS_FIN){
            fin = true;
            send_bytes = Stareas.Send(socket_fd, MENS_FIN);
        }
    } while(!fin);

    // Cerramos el socket
    int error_code = Smatriz.Close();
    int error_code2 = Stareas.Close();
    if(error_code == -1 || error_code2 == -1) {
        cerr << Stareas.error("Error cerrando el socket");
        cerr << Smatriz.error("Error cerrando el socket");
    }

    // Despedida
    cout << "Bye bye" << endl;
}

int main(int argc,char* argv[]) {
    
    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS = argv[1]; //alternativamente "127.0.0.1"
    int SERVER_PORT = stoi(argv[2]);
    thread cliente[N_CONTROLLERS];
    
    for(int i = 0; i < N_CONTROLLERS; i++) {
        cliente[i] = thread(fcliente, SERVER_PORT, SERVER_ADDRESS);
    }
    for(int i = 0; i < N_CONTROLLERS; i++) {
        cliente[i].join();
    }
    return 0;
}
