#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <cstdlib>
#include <cmath>

using namespace std;

void cargarV(float v[], int N, bool& fin_carga){
    for (int i = 0; i < N; i++){
        v[i] =1 + rand() % 1000;
    }
    fin_carga = true;
}

void CalMedia(const float v[], float& media, int N, bool& fin_carga, bool& fin_media){
    while(!fin_carga){}
    float sum = 0;
    for(int i = 0; i < N;i++){
        sum += v[i];
    }
    media = sum / N;
    fin_media = true;
}

void calcularMinimo(const float v[], bool& fin_carga, int N, float &minimo) {
    while(!fin_carga){}
    minimo = v[0];
    for (int i = 1; i < N; i++) {
        if (v[i] < minimo) {
            minimo = v[i];
        }
    }
}

void calcularMaximo(const float v[], bool& fin_carga, int N, float &maximo) {
    while(!fin_carga){}
    maximo = v[0];
    for (int i = 1; i < N; i++) {
        if (v[i] > maximo) {
            maximo = v[i];
        }
    }
}

void calcularDesviacion(const float v[], int N, float& media, float &desviacion, bool& fin_media) {
    while(!fin_media){}
    double suma = 0.0;
    for (int i = 0; i < N; i++) {
        suma += pow(v[i] - media, 2);
    }
    desviacion = sqrt(suma / N);  // fórmula de la desviación típica
}

int main(){
    srand(12);
    int N = 100;
    float v[N] = {0};
    bool fin_carga = false, fin_media = false;
    float min = 0, max = 0, media = 0, desviacion = 0;


    thread cargar(cargarV, &v[0] ,N , ref(fin_carga));

    thread Cmin(calcularMinimo,&v[0],ref(fin_carga), N, ref(min)); 
    
    thread Cmax(calcularMaximo,&v[0], ref(fin_carga), N, ref(max));

    thread Cmedia(CalMedia,&v[0], ref(media), N, ref(fin_carga), ref(fin_media));

    thread des(calcularDesviacion,&v[0], N, ref(media), ref(desviacion), ref(fin_media));

    cargar.join();
    Cmin.join();
    Cmax.join();
    Cmedia.join();
    des.join();

    cout << "La media es: " << media << endl;
    cout << "El minimo es: " << min << endl;
    cout << "El maximo es: " << max << endl;
    cout << "La desviación es: " << desviacion << endl;
}