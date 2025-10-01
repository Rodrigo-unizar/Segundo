#include "agenda.hpp"
#include "contacto.hpp"
#include "main.hpp"

using namespace std;

void iniciar(agenda& a){
    a.total = 0;
    return;
}

/* Si la agenda a no esta llena (numero de contactos almacenados menor que MAX),
   la funcion devuelve false y añade el contacto c a la agenda a.
   Si la agenda esta llena, la funcion devuelve true y la agenda a no se modifica.
*/
bool anyadir(agenda& a, const contacto& c){
    if(a.total < MAX){
        a.datos[a.total] = c;
        a.total ++;
        return false;
    }else{
        return true;
    }
}

/* Devuelve true si y solo si la agenda a esta vacia. 
*/
bool vacia(const agenda& a){
    return a.total == 0;
}

/* Si a no esta vacia, la funcion devuelve la agenda modificada eliminando el ultimo contacto 
   añadido a ella. Si a esta vacia, la funcion devuelve la agenda sin modificar.
*/
void borrarUltimo(agenda& a){
    a.total --;
    return;
}

/* Dada una agenda a y un contacto c, devuelve true si y solo si en 
   a hay algun contacto igual a c (en el sentido de la funcion == del TAD contacto), 
   false en caso contrario.
 */
bool esta(const agenda& a, const contacto& c){
    bool iguales = false;
    for(int i = 0; i < a.total && !iguales; i ++){
        iguales = operator==(c, a.datos[i]);
    }
    return iguales;
}

void iniciarIt(agenda& a){
    a.iterador = 0;
    return;
}

bool existeSig(agenda& a){
    if(a.iterador < a.total){
        return true;
    }else{
        a.iterador = 0;
        return false;
    }
    
}

bool siguienteYavanza(agenda& a, contacto& c){
    c = a.datos[a.iterador];
    a.iterador ++;
    return true;
}