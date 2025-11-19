#include <iostream>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <thread>
#include "librerias\MultiBuffer\MultiBuffer.hpp"
#include <string>
#include <iomanip>
#include <mutex>
#include <condition_variable>

using namespace std;
const int N_CONTROLLERS = 10;

struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};

struct monitor{
    public:
        monitor();
        void asignarTarea( tarea T);
        void asignarTareaFinal(tarea TF);
        void mostrarMatriz();
        void tomarTarea(unsigned int id, tarea& T);
        void escribirMatriz(tarea T, bool exito);
    private:
        MultiBuffer<tarea,N_CONTROLLERS> mBT; //multi-buffer de tareas
        float matriz[3][3];
        mutex mtx;
        condition_variable c1; //B.numberNews() < N_CONTROLLERS
        condition_variable c2; //B.numberNews() == 0
        condition_variable cC[N_CONTROLLERS]; //B.isNew(i)
        
};