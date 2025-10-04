/* Rodrigo Herce Alonso 935413
*/


#include "contacto.hpp"


#include <iostream>
using namespace std;

void crear(string nombre, string direccion, int telefono, contacto& c){
    c.nombre = nombre;
    c.direccion = direccion;
    c.telefono = telefono;
}

string nombre(const contacto& c){
    return c.nombre;
}

string direccion(const contacto& c){
    return c.direccion;
}

int telefono(const contacto& c){
    return c.telefono;
}

bool operator==(const contacto& c1, const contacto& c2){
    return c1.nombre == c2.nombre;
}

