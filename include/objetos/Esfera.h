#ifndef ESFERA_H
#define ESFERA_H

#include "./auxiliares/Pontos.h"
#include "./auxiliares/Iluminacao.h"
#include "./auxiliares/Ray.h"
#include "./auxiliares/Vetores.h"
#include <cmath>
#include <algorithm>

// ============== DEFINIÇÃO DA CLASSE ==============
class Esfera{
public:
    // características da esfera
    float rEsfera;
    Ponto cEsfera;
    Cor esfColor;
    // propriedades da esfera
    Propriedades propriedades_esfera;   
    int m_esfera;
    // construtor da esfera
    Esfera(float raio, Ponto centro, Cor cor, Propriedades propriedades, int m);

};

// ============== MÉTODOS ASSOCIADOS A ESFERA ==============

bool IntersecaoRayEsfera(Esfera esfera, Ray ray, float &ti);

#endif
