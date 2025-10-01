/* Nombre, apellidos y NIP del (de los) autor(es).
*/

#ifndef AGRUPACION_HPP
#define AGRUPACION_HPP

// PREDECLARACION DEL TAD GENERICO agrupacion (inicio INTERFAZ)

const int MAX = 40;

/* COMPLETAR CON:
   LA ESPECIFICACION DEL DOMINIO DE VALORES DEL TAD agrupacion
   Y CON LAS CONDICIONES QUE SE LE EXIGEN AL PARAMETRO DE TIPO T.
   ...
*/
template<typename T> struct agrupacion;


/* COMPLETAR LA DOCUMENTACION PUBLICA DE CADA OPERACION DEL TAD
*/
template<typename T> void iniciar(agrupacion<T>& a);

/* ... documentar ...
*/
template<typename T> bool anyadir(agrupacion<T>& a, const T& e);

/* ... documentar ...
*/
template<typename T> bool vacia(const agrupacion<T>& a);

/* ... documentar ...
*/
template<typename T> void borrarUltimo(agrupacion<T>& a);

/* ... documentar ...
*/
template<typename T> bool esta(const agrupacion<T>& a, const T& e);

//COMPLETAR OPERACIONES ITERADOR
//....
template<typename T> void iniciarIt(agrupacion<T>& a);
template<typename T> bool existeSig(const agrupacion<T>& a);
template<typename T> bool siguienteYavanza(agrupacion<T>& a, T& c);

// FIN predeclaracion del TAD GENERICO agrupacion (Fin INTERFAZ)



// DECLARACION DEL TAD GENERICO agrupacion

template<typename T>
struct agrupacion{
    friend void iniciar<T>(agrupacion<T>& a);
    friend bool anyadir<T>(agrupacion<T>& a, const T& e);
    friend bool vacia<T>(const agrupacion<T>& a);
    friend void borrarUltimo<T>(agrupacion<T>& a);
    friend bool esta<T>(const agrupacion<T>& a, const T& e);
    //COMPLETAR OPERACIONES ITERADOR
    friend void iniciarIt<T>(agrupacion<T>& a);
    friend bool existeSig<T>(const agrupacion<T>& a);
    friend bool siguienteYavanza<T>(agrupacion<T>& a, T& c);
    //....
  private: //declaracion de la representacion interna del tipo
           //... a COMPLETAR CON documentacion sobre la representacion interna ...
     T datos[MAX];
     int total;
     int iterador;
    //COMPLETAR CAMPOS ITERADOR
    //...
};


// IMPLEMENTACION DE LAS OPERACIONES DEL TAD GENERICO agrupacion

 //...documentar ...
template<typename T>
void iniciar(agrupacion<T>& a) {
    // COMPLETAR IMPLEMENTACION
    a.total = 0;
    return;
}

//...documentar ...
template<typename T>
bool anyadir(agrupacion<T>& a, const T& e) {
    // COMPLETAR IMPLEMENTACION
    if (a.total < MAX){
        a.datos[a.total] = e;
        a.total ++;
        return false;
    }
    return true;
}

//...documentar ...
template<typename T>
bool vacia(const agrupacion<T>& a) {
    // COMPLETAR IMPLEMENTACION
    return a.total == 0;
}
	
//...documentar ...	
template<typename T>
void borrarUltimo(agrupacion<T>& a) {
    // COMPLETAR IMPLEMENTACION
    if(!vacia(T)){a.total --;}
    return;
}

//...documentar ...
template<typename T>
bool esta(const agrupacion<T>& a, const T& e) {
    // COMPLETAR IMPLEMENTACION (USANDO LA FUNCION == DEL TIPO T)
    for(int i = 0; i < a.total; i++){
        if(a.dato[i] == e){
            return true;
        }
    }
    return false;
}

//COMPLETAR IMPLEMENTACION OPERACIONES ITERADOR
//...
template<typename T> 
void iniciarIt(agrupacion<T>& a){
    a.iterador = 0;
    return;
}
template<typename T> 
bool existeSig(const agrupacion<T>& a){
    return(a.iterador < a.total);
}
template<typename T> 
bool siguienteYavanza(agrupacion<T>& a, T& c){
    if(existeSig(a)){
        c = a.datos[a.iterador];
        a.iterador ++;
    }
    return true;
}

#endif //fin de agrupacion.hpp
