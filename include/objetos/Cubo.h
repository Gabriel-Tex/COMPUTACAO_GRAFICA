#ifndef CUBO_H
#define CUBO_H

#include "./auxiliares/Pontos.h"
#include "./auxiliares/Vetores.h"
#include "./auxiliares/Ray.h"
#include "ListMesh.h"
#include "Triangulo.h"

class Cubo {
public:

    ListMesh mesh;
    Propriedades prop;
    int m;
    Ponto centro;
    float lado;
    
    Cubo(Ponto centro, float lado, Propriedades prop, int m);
    
    // Método para verificar interseção
    bool intersecta(Ray ray, float &ti, Triangulo*& tri_hit) const;
    
    // Método para obter normal (delega para o triângulo)
    Vetor normalNoPonto(Ponto p) const;
    
    // Getter para a mesh (opcional)
    const ListMesh& getMesh() const { return mesh; }
};

// Funções de interseção
bool IntersecaoRayCubo(Cubo cubo, Ray ray, float &ti);
Vetor normalCubo(Cubo cubo, Ponto p);

#endif