#include "objetos/Triangulo.h"

// ============== CLASSE ==============

Triangulo::Triangulo(Ponto A, Ponto B, Ponto C, Vetor n, Propriedades prop, int m){
    this->A = A;
    this->B = B;
    this->C = C;
    this->n = n;
    this->prop = prop;
    this->m = m;
}

// ============== MÉTODOS ASSOCIADOS AO TRIÂNGULO ==============

bool Triangulo::estaDentroTriangulo(const Ponto& P) const {
    // Coordenadas baricêntricas
    Vetor v0 = B - A;
    Vetor v1 = C - A;
    Vetor v2 = P - A;

    // Produtos escalares
    float d00 = produto_escalar(v0, v0);
    float d01 = produto_escalar(v0, v1);
    float d11 = produto_escalar(v1, v1);
    float d20 = produto_escalar(v2, v0);
    float d21 = produto_escalar(v2, v1);

    float denominador = d00 * d11 - d01 * d01;
    
    // Evitar divisão por zero
    if (fabs(denominador) < 1e-8f)
        return false;

    // Coordenadas baricêntricas
    float u = (d11 * d20 - d01 * d21) / denominador;
    float v = (d00 * d21 - d01 * d20) / denominador;

    // Verificar se o ponto está dentro do triângulo
    return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
}

// ============== FUNÇÃO DE INTERSEÇÃO ==============

bool IntersecaoRayTriangulo(Triangulo triangulo, Ray ray, float &ti) {
    const float EPS = 1e-6f;

    // Vetores das arestas do triângulo
    Vetor v0 = triangulo.B - triangulo.A;
    Vetor v1 = triangulo.C - triangulo.A;

    // 1. Interseção com o plano do triângulo
    float denominador = produto_escalar(ray.dr, triangulo.n);

    // Raio paralelo ao plano?
    if (fabs(denominador) < EPS) {
        return false;
    }

    // Vetor do vértice do triângulo até a origem do raio
    Vetor w = triangulo.A - ray.P_0;
    
    // Parâmetro t (distância ao longo do raio)
    float t = produto_escalar(w, triangulo.n) / denominador;

    // Interseção atrás da câmera?
    if (t < EPS) {
        return false;
    }

    // 2. Calcular ponto de interseção no plano
    Ponto P = (ray.dr * t) + ray.P_0;

    // 3. Verificar se o ponto está dentro do triângulo
    // Coordenadas baricêntricas
    Vetor v2 = P - triangulo.A;

    float d00 = produto_escalar(v0, v0);
    float d01 = produto_escalar(v0, v1);
    float d11 = produto_escalar(v1, v1);
    float d20 = produto_escalar(v2, v0);
    float d21 = produto_escalar(v2, v1);

    float denominador_b = d00 * d11 - d01 * d01;
    
    if (fabs(denominador_b) < EPS) {
        return false; // Triângulo degenerado
    }

    float u = (d11 * d20 - d01 * d21) / denominador_b;
    float v = (d00 * d21 - d01 * d20) / denominador_b;

    // Verificar condições das coordenadas baricêntricas
    if (u < 0.0f || v < 0.0f || (u + v) > 1.0f) {
        return false;
    }

    // Interseção válida
    ti = t;
    return true;
}

void normalTriangulo(Triangulo triangulo){
    Vetor v = triangulo.B - triangulo.A;
    Vetor u = triangulo.C - triangulo.A;
    triangulo.n = normalizar(produto_vetorial(v, u));
}