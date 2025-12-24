#ifndef CUBO_H
#define CUBO_H

#include "./auxiliares/Pontos.h"
#include "./auxiliares/Vetores.h"
#include "./auxiliares/Ray.h"
#include "ListMesh.h"
#include "Triangulo.h"

// ============== DEFINIÇÃO DA CLASSE ==============

class Cubo {
public:

    ListMesh mesh;
    Propriedades prop;
    int m;
    Ponto centro;
    float lado;

    // construtor
    Cubo(Ponto centro, float lado, Propriedades prop, int m);
    
    // ============== MÉTODOS ASSOCIOADOS AO CUBO ==============

    bool intersecta(Ray ray, float &ti, Triangulo*& tri_hit) const;
    
    Vetor normalNoPonto(Ponto p) const;
    
    const ListMesh& getMesh() const { return mesh; }
};

bool IntersecaoRayCubo(Cubo cubo, Ray ray, float &ti);
Vetor normalCubo(Cubo cubo, Ponto p);

#endif