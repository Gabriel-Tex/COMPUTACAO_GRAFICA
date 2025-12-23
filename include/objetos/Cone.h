#ifndef CONE_H
#define CONE_H

#include "./auxiliares/Pontos.h"
#include "./auxiliares/Iluminacao.h"
#include "./auxiliares/Ray.h"
#include "./auxiliares/Vetores.h"
#include "Plano.h"
#include <cmath>
#include <vector>

// ============== DEFINIÇÃO DA CLASSE ==============

class Cone{
public:
    Ponto CB;
    float rbCone;
    float altura;
    Vetor dco; // normal da base
    Propriedades prop;
    int m;

    // construtor
    Cone(Ponto CB, float rbCone, float altura, Vetor dco, Propriedades prop, int m);
};

// ============== MÉTODOS ASSOCIADOS A CONE ==============

bool IntersecaoRayCone(Cone cone, Ray ray, float &t);

Vetor normalCone(Cone cone, Ponto P);

#endif
