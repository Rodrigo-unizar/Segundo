#include <iostream>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <thread>
#include <Semaphore_V4.hpp>
#include <MultiBuffer.hpp>
#include <string>
#include <iomanip>
#include "monitor.hpp"



//----------------------------------------------------


//----------------------------------------------------

void masterTask(MultiBuffer<tarea, N_CONTROLLERS>& B, float matriz[3][3], monitor& m) {
    tarea T;
    bool asignada = false;
    ifstream f("tareas.txt");

    tarea TF;
    TF.tipoTarea = "TF";
    TF.cargaDeTrabajo = 0.0;

    if(f.is_open()){
        while (!f.eof()){
            getline(f, T.tipoTarea, ',');
            f >> T.cargaDeTrabajo;
            f.ignore();

            m.asignarTarea(T);
        }
    }


    m.asignarTareaFinal(TF);
    //poner tareas de fin en cada buffer

    m.mostrarMatriz();

    //mostrar matriz de resultados cuando se pueda
}

//----------------------------------------------------
//Pre:  0 <= id < N_CONTROLLERS
void controllerTask(unsigned int id, MultiBuffer<tarea, N_CONTROLLERS>& B, float matriz[3][3], monitor& m) {
    bool seguir = true;
    int num_exist;
    bool exito;
    tarea T;
    while (seguir) {
        m.tomarTarea(id, T);

        if(T.tipoTarea == "t1"){
            num_exist =1 + rand() % 100;
            exito = num_exist > 5;
            if(exito){
                this_thread::sleep_for(chrono::milliseconds(static_cast<int>(T.cargaDeTrabajo)));
            }
        }else if(T.tipoTarea == "t2"){
            num_exist =1 + rand() % 100;
            exito = num_exist > 7;
            if(exito){
                this_thread::sleep_for(chrono::milliseconds(static_cast<int>(T.cargaDeTrabajo)));
            }
        }else if(T.tipoTarea == "t3"){
            num_exist =1 + rand() % 100;
            exito = num_exist > 10;
            if(exito){
                this_thread::sleep_for(chrono::milliseconds(static_cast<int>(T.cargaDeTrabajo)));
            }
        }else if(T.tipoTarea == "TF"){
            seguir = false;
        }

        m.escribirMatriz(T, exito);
    }
}

//----------------------------------------------------
int main(int argc, char *argv[]) {

    srand(12);
    
    monitor m;
    MultiBuffer<tarea, N_CONTROLLERS> mBT;
    float matriz[3][3];

    thread master(&masterTask, ref(mBT), ref(matriz), ref(m));

    // Crear hilos controladores
    thread controladores[N_CONTROLLERS];
    for (int i = 0; i < N_CONTROLLERS; ++i) {
        controladores[i] = thread(&controllerTask, i, ref(mBT), ref(matriz), ref(m));
    }

    // Esperar a que terminen todos
    master.join();
    for (int i = 0; i < N_CONTROLLERS; ++i){
        controladores[i].join();
    }
    return 0;
}