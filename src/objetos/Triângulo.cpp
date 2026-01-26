#include "objetos/Triangulo.h"
#include <cmath>
#include <algorithm> 

Triangulo::Triangulo(Ponto A, Ponto B, Ponto C, Vetor n, Propriedades prop, int m)
    : A(A), B(B), C(C), n(normalizar(n)), prop(prop), m(m) {
    id = 0;
}

bool Triangulo::intersecta(const Ray& ray, float& ti) const {
    const float EPS = 1e-6f;

    float denominador = produto_escalar(ray.dr, this->n);

    if (fabs(denominador) < EPS) {
        return false;
    }

    Vetor w = this->A - ray.P_0;
    float t = produto_escalar(w, this->n) / denominador;

    if (t < EPS) {
        return false;
    }

    Ponto P = ray.P_0 + (ray.dr * t);
    Vetor v0 = B - A;
    Vetor v1 = C - A;
    Vetor v2 = P - A;

    float d00 = produto_escalar(v0, v0);
    float d01 = produto_escalar(v0, v1);
    float d11 = produto_escalar(v1, v1);
    float d20 = produto_escalar(v2, v0);
    float d21 = produto_escalar(v2, v1);

    float denom_b = d00 * d11 - d01 * d01;
    
    if (fabs(denom_b) < EPS) {
        return false; 
    }

    float u = (d11 * d20 - d01 * d21) / denom_b;
    float v = (d00 * d21 - d01 * d20) / denom_b;

    if (u < 0.0f || v < 0.0f || (u + v) > 1.0f) {
        return false;
    }

    ti = t;
    return true;
}

Vetor Triangulo::calcularNormal(const Ponto& p) const {
    return n;
}

void Triangulo::transforma(const Matriz4x4& M) {
    A = M * A;
    B = M * B;
    C = M * C;
    n = normalizar(M * n); 
}

bool IntersecaoRayTriangulo(Triangulo triangulo, Ray ray, float &ti) {
    return triangulo.intersecta(ray, ti);
}