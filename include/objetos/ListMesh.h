#ifndef LISTMESH_H
#define LISTMESH_H

#include <vector>
#include "./auxiliares/Pontos.h"
#include "./auxiliares/Vetores.h"
#include "Triangulo.h"

class ListMesh {
private:
    std::vector<Triangulo> triangulos;
    
public:
    ListMesh();
    
    void addTriangulo(const Triangulo& t);
    
    // Getter
    const std::vector<Triangulo>& getTriangulos() const { return triangulos; }
    
    // Função de interseção
    bool intersecta(Ray ray, float &ti, Triangulo*& tri_hit) const;
};

// Função de interseção (mantida para compatibilidade)
bool IntersecaoRayListMesh(const ListMesh& mesh, Ray ray, float& ti, Triangulo*& tri_hit);

#endif