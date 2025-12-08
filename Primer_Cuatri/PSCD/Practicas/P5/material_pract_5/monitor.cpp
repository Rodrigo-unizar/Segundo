#include "monitor.hpp"

monitor::monitor(){}

void monitor::asignarTarea(tarea T){
    unique_lock<mutex> lck(mtx);
    while (!(mBT.numberNews() < N_CONTROLLERS)) {
        c1.wait(lck);
    }
    bool asignada = false;
    int i = 0;
    while (!asignada && i < N_CONTROLLERS) {
        if (mBT.isNew(i) == false){
            mBT.put(T, i);
            asignada = true;
        }else{
            i = (i + 1);
        }
    }
    cC[i].notify_one();
}



void monitor::asignarTareaFinal(tarea TF){
    unique_lock<mutex> lck(mtx);
    while (!(mBT.numberNews() == 0)){
        c2.wait(lck);
    }
    for(int j = 0; j < N_CONTROLLERS;j++){
        mBT.put(TF, j);
        cC[j].notify_one();
    }
    
}


void monitor::tomarTarea(unsigned int id, tarea& T){
    unique_lock<mutex> lck(mtx);
    while(!mBT.isNew(id)) {
        cC[id].wait(lck);
    }
    T = mBT.get(id);
    c1.notify_one();
    c2.notify_all();
}
