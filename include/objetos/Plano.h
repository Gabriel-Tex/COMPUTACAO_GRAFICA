#ifndef PLANO_H
#define PLANO_H

#include "./auxiliares/Pontos.h"
#include "./auxiliares/Iluminacao.h"
#include "./auxiliares/Vetores.h"
#include "./auxiliares/Ray.h"
#include "./auxiliares/Textura.h"

// ============== DEFINIÇÃO DA CLASSE ==============
class Plano{
public:
    Ponto P_pi;
    Vetor n;
    Propriedades prop;
    int m;
    Textura* textura;
    bool temTextura;
    
    // Métodos para textura
    void setTextura(Textura* tex);
    Cor getCorTextura(const Ponto& Pt) const;
    
    // construtor
    Plano(Ponto P, Vetor N, Propriedades propriedades, int m);
};

// ============== MÉTODOS ASSOCIADOS A PLANO ==============

bool IntersecaoRayPlano(Plano plano, Ray ray, float &ti);


#endif
