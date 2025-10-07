//------------------------------------------------------------------------------
// File:   main_p2_e1.cpp
// Author: Rodrigo Herce Alonso
// Date:   Octubre 2025
// Coms:   Parte del práctica 2 de PSCD
//         Compilar mediante Makefile asociado
//------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;

const int N = 512;
const int N_BUSC = 8; // # de procesos buscadores

//------------------------------------------------------------------------------
// Pre: 0 <= i <= d <= N-1
// Post: result = Num alpha \in [i,d].value = v[alpha]
//------------------------------------------------------------------------------
void search(const int v[N], const int i, const int d, const int value, int &result){
	for(int j = i; j <= d; j++){
		if(v[j] == value){
			result++;
		}
	}
	return;
}

void buscador(const int orden,const int v[N], int &result, const int value, const bool& datosL, bool& fin){
	while(!datosL){
		this_thread::yield();
	}

	int i = N/N_BUSC * orden, cont = 0;

	search(v,i, i + N/N_BUSC - 1, value, result);
	cout << "proceso " + to_string(i) +  "       " + to_string(i) + "       " + to_string(i + N/N_BUSC - 1) + "\n";

	fin = true;

	return;
}

void coordinador(int v[N], bool& datosL, int result[N_BUSC],bool fin[N_BUSC], const int value){

    ifstream fichero("datos.txt");
    if (!fichero.is_open()) {
        cerr << "Error: no se pudo abrir el fichero datos.txt" << endl;
    }

    int i = 0;

    int numero;
    while (fichero >> numero && i < N) { // lee números separados por espacios o saltos
        v[i] = numero;
		//v[i]=1;
        i++;
    }

    fichero.close();
	datosL = true;
	
	for(int i = 0; i < N_BUSC ; i++){
		while(!fin[i]){
			this_thread::yield;
		}
	}

	int total = 0;

	for(int i = 0; i < N_BUSC; i++){
		total += result[i];
	}

	cout << "El numero de veces que ha aparecido el numero " + to_string(value) + ": " + to_string(total) + "\n";
}
 
int main(int argc, char* argv[]) {
	int result[N_BUSC], value = 98;
	int v[N];
	bool datosL = false, fin[N_BUSC];
	cin >> value;

	for(int i = 0; i < N_BUSC; i++){
		result[i] = 0;
		fin[i] = false;
	}
	thread C;
	thread B[N_BUSC];

	C = thread(&coordinador, v, ref(datosL), result ,fin, value);

	for(int i = 0; i < N_BUSC; i++){
		B[i] = thread(&buscador, i, v,ref(result[i]), value, ref(datosL), ref(fin[i]));
	}
	C.join();
	for(int i = 0; i < N_BUSC; i++){
		B[i].join();
	}

    return 0;
}
