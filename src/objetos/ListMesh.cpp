#include "objetos/ListMesh.h"
#include <limits>
using namespace std;

// ================= CONSTRUTOR =================

ListMesh::ListMesh() {}

// ================= MÉTODO =================

void ListMesh::addTriangulo(const Triangulo& t) {
    triangulos.push_back(t);
}

bool ListMesh::intersecta(Ray ray, float &ti, Triangulo*& tri_hit) const {
    bool hit = false;
    ti = numeric_limits<float>::max();
    tri_hit = nullptr;

    for (const auto& tri : triangulos) {
        float t_aux;

        if (IntersecaoRayTriangulo(tri, ray, t_aux)) {
            if (t_aux < ti) {
                ti = t_aux;
                tri_hit = (Triangulo*)&tri; 
                hit = true;
            }
        }
    }

    return hit;
}

void ListMesh::limpar() {
    triangulos.clear();
}

// ========== INTERSEÇÃO RAY x LISTMESH ==========

bool IntersecaoRayListMesh(
    const ListMesh& mesh,
    Ray ray,
    float& ti,
    Triangulo*& tri_hit
){
    return mesh.intersecta(ray, ti, tri_hit);
}

void ListMesh::transforma(const Matriz4x4& M) {
    for (auto& tri : triangulos) {
        tri.transforma(M);
    }
}
   

