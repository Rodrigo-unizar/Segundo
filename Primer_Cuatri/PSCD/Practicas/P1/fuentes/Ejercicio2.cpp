//------------------------------------------------------------------------------
// File:   practica_1_V2.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2020
// Coms:   Parte del práctica 1 de PSCD
//         Compilar mediante
//           g++ practica_1_V2.cpp -o practica_1_V2 -std=c++11 -pthread
//------------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <cstdlib>

using namespace std;

void saludo(int retardo, int veces) {
    for(int i=1; i<=veces; i++) {
        //cout << "Soy " << nombre << endl;
        cout << "Soy " + to_string(i) + "\n";
        //el thread que me ejecuta se bloquea durante "retardo" milisegundos
        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
}

int main(int argc, char* argv[]) {
    srand(time(0));
    const int N = 10;
    thread P[N]; //de momento, ningún thread se pone en marcha

    for(int i = 0; i < N;i ++){
        P[i] = thread(&saludo, rand() % 200 + 100, rand() % 10 + 5); //P[0] se pone en marcha
    }

    for (int i=0; i<N; i++) { 
        P[i].join(); //me bloqueo hasta que "P[i]" termine
    }

    cout << "Fin\n";
    return 0;
}
