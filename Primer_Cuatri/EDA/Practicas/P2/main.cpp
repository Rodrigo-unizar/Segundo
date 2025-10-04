#include <IOstream>

#include "agrupacion.hpp"
#include "contacto.hpp"
#include "producto.hpp"

using namespace std;

int main(){

    agrupacion<contacto> agenda;
    agrupacion<producto> prod;
    contacto c[6];
    string nombres[6] = {"Maria", "Juan", "Jaime", "Isma", "Rodri", "Belen"};
    string direcciones[6] = {"C/Alfajerias","C/De todos los Santos","C/Maria de Luna","C/Sol y Luna","C/Gayarre","C/Del Porvenir"};
    int numerosTlf[6] = {616349897,687955341,636454332,722149987,754123876,754123098};
    string nombreAbuscar;


    iniciar(agenda);
    for(int i = 0; i < 6; i++){
        crear(nombres[i], direcciones[i], numerosTlf[i], c[i]);
        anyadir(agenda, c[i]);
    }
    cout << endl;
    contacto cRec;

    iniciarIt(agenda);
    while(existeSig(agenda)){
        siguienteYavanza(agenda, cRec);
        cout << "Este es el contacto de: " << nombre(cRec) << ". Con direccion: " << direccion(cRec) << " y numero de Telefono: " << telefono(cRec) << endl;   
    }
    cout << endl;

    cin >> nombreAbuscar;

    contacto aBuscar;

    borrarUltimo(agenda);
    iniciarIt(agenda);
    while(existeSig(agenda)){
        siguienteYavanza(agenda, cRec);
        cout << "Este es el contacto de: " << nombre(cRec) << ". Con direccion: " << direccion(cRec) << " y numero de Telefono: " << telefono(cRec) << endl;   
    }

    cout << "Dame un nombre y te digo si esta entre tus contactos: " << endl;
    cin >> nombreAbuscar;

    crear(nombreAbuscar, "", 0, aBuscar);
    if(esta(agenda, aBuscar)){
        cout << nombreAbuscar << " si que pertenece a tu lista de contactos." << endl;
    }else{
        cout << nombreAbuscar << " no pertenece a tu lista de contactos." << endl;
    }

}