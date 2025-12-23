#ifndef CILINDRO_H
#define CILINDRO_H

#include "./auxiliares/Pontos.h"
#include "./auxiliares/Iluminacao.h"
#include "./auxiliares/Ray.h"
#include "./auxiliares/Vetores.h"
#include "Plano.h"
#include <cmath>
#include <algorithm>
#include <vector>

// ============== DEFINIÇÃO DA CLASSE ==============

class Cilindro{
public:
    Ponto CB;
    float rbCil;
    float altura;
    Vetor dcil;
    Propriedades prop;
    int m;

    // construtor
    Cilindro(Ponto CB, float rbCil, float altura, Vetor dcil, Propriedades prop, int m);
};

// ============== MÉTODOS ASSOCIADOS A CILINDRO ==============

bool IntersecaoRayCilindro(Cilindro cilindro, Ray ray, float &t);

Vetor normalCilindro(Cilindro cilindro, Ponto P);

#endif
