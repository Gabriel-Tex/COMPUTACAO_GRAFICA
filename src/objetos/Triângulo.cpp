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

    Vetor aresta1 = B - A;
    Vetor aresta2 = C - A;
    
    n = normalizar(produto_vetorial(aresta1, aresta2));
}

void Triangulo::transladar(float tx, float ty, float tz) {
    transforma(Transformacao::translacao(tx, ty, tz));
}

void Triangulo::escalar(float sx, float sy, float sz, Ponto ponto_fixo) {
    transforma(Transformacao::escala(sx, sy, sz, ponto_fixo));
}

void Triangulo::rotacionarX(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto centro = getCentroBaricentrico();
    Matriz4x4 T1 = Transformacao::translacao(-centro.x, -centro.y, -centro.z);
    Matriz4x4 R  = Transformacao::rotacaoX(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(centro.x, centro.y, centro.z);
    transforma(T2 * R * T1);
}

void Triangulo::rotacionarY(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto centro = getCentroBaricentrico();
    Matriz4x4 T1 = Transformacao::translacao(-centro.x, -centro.y, -centro.z);
    Matriz4x4 R  = Transformacao::rotacaoY(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(centro.x, centro.y, centro.z);
    transforma(T2 * R * T1);
}

void Triangulo::rotacionarZ(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto centro = getCentroBaricentrico();
    Matriz4x4 T1 = Transformacao::translacao(-centro.x, -centro.y, -centro.z);
    Matriz4x4 R  = Transformacao::rotacaoZ(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(centro.x, centro.y, centro.z);
    transforma(T2 * R * T1);
}

void Triangulo::rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus, Ponto ponto) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 R = Transformacao::rotacaoEixoArbitrarioPonto(eixo, anguloRad, ponto);
    transforma(R);
}

void Triangulo::cisalharX_XZ(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto centro = getCentroBaricentrico();
    Matriz4x4 T1 = Transformacao::translacao(-centro.x, -centro.y, -centro.z);
    Matriz4x4 C  = Transformacao::cisalhamentoX_XZ(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(centro.x, centro.y, centro.z);
    transforma(T2 * C * T1);
}

void Triangulo::cisalharY_XZ(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto c = getCentroBaricentrico();
    Matriz4x4 T1 = Transformacao::translacao(-c.x, -c.y, -c.z);
    Matriz4x4 C  = Transformacao::cisalhamentoY_XZ(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(c.x, c.y, c.z);
    transforma(T2 * C * T1);
}

void Triangulo::cisalharY_XY(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto c = getCentroBaricentrico();
    Matriz4x4 T1 = Transformacao::translacao(-c.x, -c.y, -c.z);
    Matriz4x4 C  = Transformacao::cisalhamentoY_XY(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(c.x, c.y, c.z);
    transforma(T2 * C * T1);
}

void Triangulo::cisalharZ_XY(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto c = getCentroBaricentrico();
    Matriz4x4 T1 = Transformacao::translacao(-c.x, -c.y, -c.z);
    Matriz4x4 C  = Transformacao::cisalhamentoZ_XY(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(c.x, c.y, c.z);
    transforma(T2 * C * T1);
}

void Triangulo::espelharXY() { 
    transforma(Transformacao::espelhamentoXY()); 
}
void Triangulo::espelharXZ() { 
    transforma(Transformacao::espelhamentoXZ()); 
}
void Triangulo::espelharYZ() { 
    transforma(Transformacao::espelhamentoYZ()); 
}

bool IntersecaoRayTriangulo(Triangulo triangulo, Ray ray, float &ti) {
    return triangulo.intersecta(ray, ti);
}