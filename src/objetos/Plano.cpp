#include "objetos/Plano.h"
#include <cmath>

Plano::Plano(Ponto P, Vetor N, Propriedades propriedades, int m)
    : P_pi(P), n(normalizar(N)), prop(propriedades), m(m),
      textura(nullptr), temTexturaFlag(false) {
    id = 0;
}

bool Plano::intersecta(const Ray& ray, float& ti) const {
    float denominador = produto_escalar(ray.dr, n);
    
    if (fabs(denominador) < 1e-6) {
        return false;
    }
    
    Vetor w = P_pi - ray.P_0;
    float t = produto_escalar(w, n) / denominador;
    
    if(t > 0) {
        ti = t;
        return true;
    }
    return false;
}

Vetor Plano::calcularNormal(const Ponto& ponto) const {
    (void)ponto; 
    return n;
}

Propriedades Plano::getPropriedades() const {
    return prop;
}

int Plano::getMaterial() const {
    return m;
}

std::string Plano::getNome() const {
    return "Plano";
}

int Plano::getId() const {
    return id;
}

Cor Plano::getCorTextura(const Ponto& Pt) const {
    if (!temTexturaFlag || textura == nullptr) {
        return prop.Kdif;
    }
    
    return getCorTexturaPt(Pt);
}

bool Plano::temTextura() const {
    return temTexturaFlag;
}

void Plano::setTextura(Textura* tex) {
    textura = tex;
    temTexturaFlag = (tex != nullptr);
}

Cor Plano::getCorTexturaPt(const Ponto& Pt) const {
    if (!temTexturaFlag || textura == nullptr) {
        return prop.Kdif;
    }
    
    if (fabs(n.y) > 0.9f) {
        float u = (Pt.x - P_pi.x) / 100.0f;
        float v = (Pt.z - P_pi.z) / 100.0f;
        return textura->amostrarRepetir(u, v);
    }
    else if (fabs(n.x) > 0.9f) {
        float u = (Pt.y - P_pi.y) / 100.0f;
        float v = (Pt.z - P_pi.z) / 100.0f;
        return textura->amostrarRepetir(u, v);
    }
    else if (fabs(n.z) > 0.9f) {
        float u = (Pt.x - P_pi.x) / 100.0f;
        float v = (Pt.y - P_pi.y) / 100.0f;
        return textura->amostrarRepetir(u, v);
    }
    
    return prop.Kdif;
}

// ========== IMPLEMENTAÇÕES DAS TRANSFORMAÇÕES ==========

void Plano::transforma(const Matriz4x4& M) {
    P_pi = M * P_pi;
    n = normalizar(M * n);
}
void Plano::transladar(float tx, float ty, float tz) {
    Matriz4x4 T = Transformacao::translacao(tx, ty, tz);
    transforma(T);
}


void Plano::escalar(float sx, float sy, float sz, Ponto ponto_fixo) {
    Matriz4x4 S = Transformacao::escala(sx, sy, sz, ponto_fixo);
    P_pi = S * P_pi;
}

void Plano::rotacionarX(float angulo) {
    Matriz4x4 R = Transformacao::rotacaoX(angulo);
    transforma(R);
}

void Plano::rotacionarY(float angulo) {
    Matriz4x4 R = Transformacao::rotacaoY(angulo);
    transforma(R);
}

void Plano::rotacionarZ(float angulo) {
    Matriz4x4 R = Transformacao::rotacaoZ(angulo);
    transforma(R);
}

