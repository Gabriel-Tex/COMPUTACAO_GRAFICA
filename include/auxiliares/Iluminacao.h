#ifndef ILUMINACAO_H
#define ILUMINACAO_H

#include "Pontos.h"
#include "Ray.h"
#include "Vetores.h"
#include <algorithm>
// ============== DEFINIÇÃO DAS CLASSES ==============

class Cor
{
public:
    float r, g, b;
    Cor() : r(0), g(0), b(0) {}
    Cor(float r, float g, float b);
};

class Propriedades{
public:
    // três propriedades do material
    Cor Kdif, Kesp, Kamb;
    // construtor
    Propriedades()
    : Kdif(0.0f, 0.0f, 0.0f),
      Kesp(0.0f, 0.0f, 0.0f),
      Kamb(0.0f, 0.0f, 0.0f) {}
    Propriedades(Cor Kd, Cor Ke, Cor Ka); 

};

class IluminacaoCena{
public:
    // IF, IA e PF
    Cor intensidade_da_fonte;
    Cor intensidade_ambiente;
    Ponto ponto_da_fonte;

    // construtor
    IluminacaoCena(Cor IF, Cor IA, Ponto PF);
};

// ============== MÉTODOS ASSOCIADOS A COR ==============

// mantém os valores de r,g,b entre 0 e 1
Cor normalizaRGB(int r, int g, int b);

// operação arroba
Cor arroba(Cor A, Cor B);

Cor CorResultante(Ponto Pt, Vetor n, Ray ray, Propriedades prop,
    IluminacaoCena iluminacao, int m, int objeto);

#endif