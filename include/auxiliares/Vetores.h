#ifndef VETORES_H
#define VETORES_H

#include "Pontos.h"
#include <cmath>

// ============== DEFINIÇÃO DA CLASSE ==============

class Vetor
{
public:
    float x, y, z;
    Vetor() : x(0.0f), y(0.0f), z(0.0f) {}
    Vetor(float x, float y, float z);
};


// ============== MÉTODOS ASSOCIADOS A VETORES ==============

/*
PF -> ponto final
PI -> ponto inicial
PF - PI = VETOR na direção PI -> PF
*/
Vetor operator-(const Ponto &PF, const Ponto &PI);

// inverter sentido de vetor 
Vetor operator-(const Vetor &V);

Ponto operator+(const Vetor &V, const Ponto &U);

Vetor operator*(const Vetor &V, const float &u);

// produto escalar entre dois vetores
float produto_escalar(Vetor V1, Vetor V2);

// ||v|| = sqrt(x^2 + y^2 + z^2)
// normalização: v/||v||
Vetor normalizar(Vetor V);

float comprimento(Vetor V);

#endif