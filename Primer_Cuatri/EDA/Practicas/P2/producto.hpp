
#ifndef PRODUCTO_HPP
#define PRODUCTO_HPP

#include<iostream>
using namespace std;

struct producto;

void crear (string nom, int can, producto& p); //…


string nombre (const producto& p); //…


int cantidad (const producto& p); //…


bool operator== (const producto& p1, const producto& p2);

struct producto{
    friend void crear (string nom, int can, producto& p);
    friend string nombre (const producto& p);
    friend int cantidad (const producto& p);
    friend bool operator== (const producto& p1, const producto& p2);

    private:
        string nombre;
        int cantidad;


};

#endif