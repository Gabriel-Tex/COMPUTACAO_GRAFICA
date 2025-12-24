#ifndef LISTMESH_H
#define LISTMESH_H

#include <vector>
#include "./auxiliares/Pontos.h"
#include "./auxiliares/Vetores.h"
#include "Triangulo.h"

// ============== DEFINIÇÃO DA CLASSE ==============

class ListMesh {
private:
    std::vector<Triangulo> triangulos;
    
public:
    ListMesh();
    
    void addTriangulo(const Triangulo& t);
    
    const std::vector<Triangulo>& getTriangulos() const { return triangulos; }
    
    bool intersecta(Ray ray, float &ti, Triangulo*& tri_hit) const;
};

bool IntersecaoRayListMesh(const ListMesh& mesh, Ray ray, float& ti, Triangulo*& tri_hit);

#endif