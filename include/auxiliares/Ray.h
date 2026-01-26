#ifndef RAY_H
#define RAY_H

#include "Pontos.h"
#include "Vetores.h"

// ============== DEFINIÇÃO DA CLASSE ==============

class Ray{
public:
    Ponto P_0; // ponto de partida do ray
    Vetor dr; // vetor direção normalizado do ray

    Ray(Ponto P_0, Vetor dr);
};

// ============== MÉTODOS ASSOCIADOS A RAY ==============


#endif