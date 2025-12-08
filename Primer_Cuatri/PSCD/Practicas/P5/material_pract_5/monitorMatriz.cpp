#include "monitorMatriz.hpp"

monitorM::monitor(){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            matriz[i][j] = 0.0;
        }
    }
}

void monitorM::mostrarMatriz(){
    unique_lock<mutex> lck(mtx);
    while (!(mBT.numberNews() == 0)){
        c2.wait(lck);
    }
    cout << "\n-----------------------------------------" << endl;
    cout << "             MATRIZ DE RESULTADOS         " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "TipoTarea | Total | Exitosas | CargaTotal" << endl;
    cout << "-----------------------------------------" << endl;

    const string nombresTareas[3] = {"t1", "t2", "t3"};

    for (int i = 0; i < 3; ++i) {
        cout << setw(9) << nombresTareas[i] << " | "
            << setw(5) << fixed << setprecision(0) <<matriz[i][0] << " | "
            << setw(8) << fixed << setprecision(0) << matriz[i][1] << " | "
            << setw(10) << fixed << setprecision(2) << matriz[i][2]
            << endl;
    }

    cout << "-----------------------------------------" << endl;
}


void monitorM::escribirMatriz(tarea T, bool exito){
    unique_lock<mutex> lck(mtx);
    if(T.tipoTarea == "t1"){
            matriz[0][0]++;
            if(exito){
                matriz[0][1]++;
            }
            matriz[0][2] += T.cargaDeTrabajo;
    }else if(T.tipoTarea == "t2"){
            matriz[1][0]++;
            if(exito){
                matriz[1][1]++;
            }
            matriz[1][2] += T.cargaDeTrabajo;
    }else if(T.tipoTarea == "t3"){
            matriz[2][0]++;
            if(exito){
                matriz[2][1]++;
            }
            matriz[2][2] += T.cargaDeTrabajo;
    }
}