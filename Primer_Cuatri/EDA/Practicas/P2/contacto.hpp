/* Rodrigo Herce Alonso 935413
*/

#ifndef CONTACTO_HPP
#define CONTACTO_HPP

#include<iostream>
using namespace std;

// PREDECLARACION DEL TAD contacto (inicio INTERFAZ)

/* Los valores del TAD contacto representan informacion de personas,
a las que llamamos contactos, para las que se tiene 
informacion de su nombre, su direccion y su numero de telefono.
Dos contactos se consideran iguales (con el operador ==) si tienen el mismo nombre.
*/
struct contacto;

/* Dada una cadena nombre, una cadena direccion y un entero telefono, 
devuelve un contacto c con esos datos. 
*/
void crear(string nombre, string direccion, int telefono, contacto& c);

/* Dado un contacto c, devuelve la cadena correspondiente al nombre de c. 
*/
string nombre(const contacto& c);

/* Dado un contacto c, devuelve la cadena correspondiente a la direccion de c. 
*/
string direccion(const contacto& c);

/* Dado un contacto c, devuelve el entero correspondiente al telefono de c. 
*/
int telefono(const contacto& c);

/* Devuelve verdad si y sólo si los contactos c1 y c2 tienen el mismo nombre. 
*/

bool operator==(const contacto& c1, const contacto& c2);


// FIN de la PREDECLARACION DEL TAD contacto (fin INTERFAZ)

// DECLARACION DEL TAD contacto 

struct contacto {
  friend void crear(string nombre, string direccion, int telefono, contacto& c);
  friend string nombre(const contacto& c);
  friend string direccion(const contacto& c);
  friend int telefono(const contacto& c);
  friend bool operator==(const contacto& c1, const contacto& c2);
  private:  //declaracion de la representacion interna del tipo
            //... a completar COMPLETAR CON documentacion sobre la representacion interna ...
    string nombre;
    string direccion;
    int telefono;
};

#endif
