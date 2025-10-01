#include <IOstream>

#include "agenda.hpp"
#include "contacto.hpp"


using namespace std;

int main(){
    agenda a;
    contacto c[6];
    string nombres[6] = {"Maria", "Juan", "Jaime", "Isma", "Rodri", "Belen"};
    string direcciones[6] = {"C/Alfajerias","C/De todos los Santos","C/Maria de Luna","C/Sol y Luna","C/Gayarre","C/Del Porvenir"};
    int numerosTlf[6] = {616349897,687955341,636454332,722149987,754123876,754123098};
    string nombreAbuscar;


    iniciar(a);
    iniciarIt(a);
    for(int i = 0; i < 6; i++){
        crear(nombres[i], direcciones[i], numerosTlf[i], c[i]);
        anyadir(a, c[i]);
    }
    cout << endl;
    contacto cRec;
    while(existeSig(a)){
        siguienteYavanza(a, cRec);
        cout << "Este es el contacto de: " << nombre(cRec) << ". Con direccion: " << direccion(cRec) << " y numero de Telefono: " << telefono(cRec) << endl;   
    }
    cout << endl;

    cin >> nombreAbuscar;

    contacto aBuscar;
    crear(nombreAbuscar, "", 0, aBuscar);
    if(esta(a, aBuscar)){
        cout << nombreAbuscar << " si que pertenece a tu lista de contactos." << endl;
    }else{
        cout << nombreAbuscar << " no pertenece a tu lista de contactos." << endl;
    }

    borrarUltimo(a);
    while(existeSig(a)){
        siguienteYavanza(a, cRec);
        cout << "Este es el contacto de: " << nombre(cRec) << ". Con direccion: " << direccion(cRec) << " y numero de Telefono: " << telefono(cRec) << endl;   
    }

    cout << "Dame un nombre y te digo si esta entre tus contactos: " << endl;
    cin >> nombreAbuscar;

    crear(nombreAbuscar, "", 0, aBuscar);
    if(esta(a, aBuscar)){
        cout << nombreAbuscar << " si que pertenece a tu lista de contactos." << endl;
    }else{
        cout << nombreAbuscar << " no pertenece a tu lista de contactos." << endl;
    }


}