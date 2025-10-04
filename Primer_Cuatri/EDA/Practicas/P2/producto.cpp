#include "producto.hpp"

void crear (string nom, int can, producto& p){
    p.nombre = nom;
    p.cantidad = can;
    return;
}


string nombre (const producto& p){
    return p.nombre;
}


int cantidad (const producto& p){
    return p.cantidad;
}


bool operator== (const producto& p1, const producto& p2){
    return(p1 == p2);
}
