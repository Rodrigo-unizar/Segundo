#include <iostream>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <thread>
#include <Semaphore_V4.hpp>
#include <MultiBuffer.hpp>
#include <string>
#include <iomanip>


using namespace std;
const int N_CONTROLLERS = 10;
//----------------------------------------------------
struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};

//----------------------------------------------------
void pasarTestigo(Semaphore& testigo, Semaphore* B[], int d[], MultiBuffer<tarea,N_CONTROLLERS>& mBT) {
    
    bool encontrado = false;
    unsigned i = 0;
    if((mBT.numberNews() < N_CONTROLLERS) && (d[0] > 0)){
        d[0]--;
        B[0]->signal();
    }else if(mBT.numberNews() == 0 && (d[1] > 0)){
        d[1]--;
        B[1]->signal();
    }else{
        while(!encontrado && (i < N_CONTROLLERS)){
            if(mBT.isNew(i) && (d[i+2] > 0)){
                encontrado = true;
            }else{
                i++;
            }
        }
        if(encontrado){
            d[i+2]--;
            B[i+2]->signal();
        }else{
            testigo.signal();
        }
    }

}

void mostrarMatriz(float matriz[3][3]) {
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

void masterTask(MultiBuffer<tarea, N_CONTROLLERS>& B, Semaphore& testigo, Semaphore* Bj[], int d[], float matriz[3][3]) {
    tarea T;
    int i = 0, terminadas = 0;
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

            /*
            <await (B.numberNews < N_CONTROLLERS)
                while(!asignada){
                    asignada = B.isNew(i);  
                    i++;
                }
                B.put(T,i);
                i = 0;
                asignada = false;
            >;*/
            
            testigo.wait();
            if (!(B.numberNews() < N_CONTROLLERS)) {
                d[0]++;
                testigo.signal();
                Bj[0]->wait();
            }
            asignada = false;
            i = 0;
            while (!asignada && i < N_CONTROLLERS) {
                if (B.isNew(i) == false){
                    B.put(T, i);
                    asignada = true;
                }else{
                    i = (i + 1);
                }
            }
            //cout << ("put "+ to_string(i)+" " + T.tipoTarea + " "+ to_string(T.cargaDeTrabajo)+ " " + "\n");
            pasarTestigo(testigo, Bj, d, B);
            
            

        }
    }

    /*<await B.numNews == 0
        for(int j = 0; j < N_CONTROLLERS;j++){
            B.put(TF, j);
        }
    >;*/
    testigo.wait();
    if(!(B.numberNews() == 0)){
        d[1]++;
        testigo.signal();
        Bj[1]->wait();
    }
    for(int j = 0; j < N_CONTROLLERS;j++){
        B.put(TF, j);
    }
    pasarTestigo(testigo, Bj, d, B);
    //poner tareas de fin en cada buffer

    /*<await B.numNews == 0
        mostrarMatriz
    >;*/
    testigo.wait();
    if(!(B.numberNews() == 0)){
        d[1]++;
        testigo.signal();
        Bj[1]->wait();
    }
    mostrarMatriz(matriz);
    pasarTestigo(testigo, Bj, d, B);
    
    //mostrar matriz de resultados cuando se pueda
}

//----------------------------------------------------
//Pre:  0 <= id < N_CONTROLLERS
void controllerTask(unsigned int id, MultiBuffer<tarea, N_CONTROLLERS>& B, float matriz[3][3], Semaphore& testigo, Semaphore* Bj[], int d[]) {
    bool seguir = true;
    int i = 0, num_exist;
    bool exito;
    tarea T;
    while (seguir) {
        //tomar tarea del buffer correspondiente cuando la haya
        /*<await B.isNew(id)
            T = B.get(id);
        >*/
        testigo.wait();
        if(!B.isNew(id)) {
            d[id + 2]++;
            testigo.signal();
            Bj[id + 2]->wait();
        }
        T = B.get(id);
        //cout << ("get "+ to_string(id)+ "\n");
        pasarTestigo(testigo, Bj, d, B);
        

        //ejecutarla
        //cout << (T.tipoTarea + "\n");
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

        //completar datos en la matriz de resultados
        /*
        <await true

            if(T.tipoTarea == "t1"){
                matriz[0][0]++;
                if(exito){
                    matriz[0][1]++;
                }
                matriz[0][2] ++ T.cargaDeTrabajo;
            }else if(T.tipoTarea == "t2"){
                matriz[1][0]++;
                if(exito){
                    matriz[1][1]++;
                }
                matriz[1][2] ++ T.cargaDeTrabajo;
            }else if(T.tipoTarea == "TF"){
                matriz[2][0]++;
                if(exito){
                    matriz[2][1]++;
                }
                matriz[2][2] ++ T.cargaDeTrabajo;
            }
        
        >
        */
        testigo.wait();
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
        pasarTestigo(testigo, Bj, d, B);
    }
}

//----------------------------------------------------
int main(int argc, char *argv[]) {

    MultiBuffer<tarea,N_CONTROLLERS> mBT; //multi-buffer de tareas
    srand(12);
    float matriz[3][3];
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            matriz[i][j] = 0.0;
        }
    }
    Semaphore testigo(1);
    Semaphore* B[N_CONTROLLERS + 2];
    int d[N_CONTROLLERS + 2];
    for (int i = 0; i < N_CONTROLLERS + 2; ++i) {
        B[i] = new Semaphore(0);
        d[i] = 0;
    }

    thread master(&masterTask, ref(mBT), ref(testigo), B, d, matriz);

    // Crear hilos controladores
    thread controladores[N_CONTROLLERS];
    for (int i = 0; i < N_CONTROLLERS; ++i) {
        controladores[i] = thread(&controllerTask, i, ref(mBT), matriz, ref(testigo), B, d);
    }

    // Esperar a que terminen todos
    master.join();
    for (int i = 0; i < N_CONTROLLERS; ++i){
        controladores[i].join();
    }
    for (int i = 0; i < N_CONTROLLERS + 2; i++) {
        delete B[i];
    }

    return 0;
}