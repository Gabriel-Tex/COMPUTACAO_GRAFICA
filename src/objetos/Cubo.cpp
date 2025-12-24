#include "objetos/Cubo.h"
#include <algorithm>

// ================= CONSTRUTOR =================

Cubo::Cubo(Ponto centro, float lado, Propriedades prop, int m)
{
    this->centro = centro;
    this->lado = lado;
    this->prop = prop;
    this->m = m;

    float h = lado / 2.0f;

    // ======== VÉRTICES ========
    Ponto v0(centro.x - h, centro.y - h, centro.z - h);
    Ponto v1(centro.x + h, centro.y - h, centro.z - h);
    Ponto v2(centro.x + h, centro.y + h, centro.z - h);
    Ponto v3(centro.x - h, centro.y + h, centro.z - h);

    Ponto v4(centro.x - h, centro.y - h, centro.z + h);
    Ponto v5(centro.x + h, centro.y - h, centro.z + h);
    Ponto v6(centro.x + h, centro.y + h, centro.z + h);
    Ponto v7(centro.x - h, centro.y + h, centro.z + h);

    // ======== NORMAIS DAS FACES ========
    Vetor n_frente ( 0,  0,  1);
    Vetor n_tras   ( 0,  0, -1);
    Vetor n_direita( 1,  0,  0);
    Vetor n_esquerda(-1, 0,  0);
    Vetor n_cima   ( 0,  1,  0);
    Vetor n_baixo  ( 0, -1,  0);

    // ======== FACE FRENTE (z+) ========
    mesh.addTriangulo(Triangulo(v4, v5, v6, n_frente, prop, m));
    mesh.addTriangulo(Triangulo(v4, v6, v7, n_frente, prop, m));

    // ======== FACE TRÁS (z-) ========
    mesh.addTriangulo(Triangulo(v0, v2, v1, n_tras, prop, m));
    mesh.addTriangulo(Triangulo(v0, v3, v2, n_tras, prop, m));

    // ======== FACE DIREITA (x+) ========
    mesh.addTriangulo(Triangulo(v1, v2, v6, n_direita, prop, m));
    mesh.addTriangulo(Triangulo(v1, v6, v5, n_direita, prop, m));

    // ======== FACE ESQUERDA (x-) ========
    mesh.addTriangulo(Triangulo(v0, v7, v3, n_esquerda, prop, m));
    mesh.addTriangulo(Triangulo(v0, v4, v7, n_esquerda, prop, m));

    // ======== FACE CIMA (y+) ========
    mesh.addTriangulo(Triangulo(v3, v7, v6, n_cima, prop, m));
    mesh.addTriangulo(Triangulo(v3, v6, v2, n_cima, prop, m));

    // ======== FACE BAIXO (y-) ========
    mesh.addTriangulo(Triangulo(v0, v1, v5, n_baixo, prop, m));
    mesh.addTriangulo(Triangulo(v0, v5, v4, n_baixo, prop, m));
}

// ================= MÉTODOS =================

bool Cubo::intersecta(Ray ray, float &ti, Triangulo*& tri_hit) const {
    return mesh.intersecta(ray, ti, tri_hit);
}

Vetor Cubo::normalNoPonto(Ponto p) const {
    return Vetor(0, 1, 0);
}

// ========== FUNÇÕES DE INTERSEÇÃO ==========

bool IntersecaoRayCubo(Cubo cubo, Ray ray, float &ti) {
    Triangulo* tri_hit = nullptr;
    return cubo.intersecta(ray, ti, tri_hit);
}

Vetor normalCubo(Cubo cubo, Ponto p) {
    
    Vetor v = p - cubo.centro;
    
    float absX = fabs(v.x);
    float absY = fabs(v.y);
    float absZ = fabs(v.z);
    
    if (absX >= absY && absX >= absZ) {
        return Vetor((v.x > 0) ? 1.0f : -1.0f, 0.0f, 0.0f);
    } else if (absY >= absX && absY >= absZ) {
        return Vetor(0.0f, (v.y > 0) ? 1.0f : -1.0f, 0.0f);
    } else {
        return Vetor(0.0f, 0.0f, (v.z > 0) ? 1.0f : -1.0f);
    }
}