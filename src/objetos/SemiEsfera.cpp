#include "objetos/SemiEsfera.h"
#include <cmath>

SemiEsfera::SemiEsfera(Ponto centro, float raio, Vetor direcao, Propriedades props, int m, bool temBase)
    : centro(centro), raio(raio), direcao(normalizar(direcao)), 
      props(props), m(m), temBase(temBase) {}

bool SemiEsfera::intersecta(const Ray& ray, float& t) const {
    const float EPS = 1e-4f;
    std::vector<float> ts_validos;

    Vetor w = ray.P_0 - centro;
    float a = produto_escalar(ray.dr, ray.dr);
    float b = 2.0f * produto_escalar(w, ray.dr);
    float c = produto_escalar(w, w) - raio * raio;
    float delta = b * b - 4.0f * a * c;

    if (delta >= 0) {
        float sqrtDelta = sqrt(delta);
        float t1 = (-b - sqrtDelta) / (2.0f * a);
        float t2 = (-b + sqrtDelta) / (2.0f * a);

        // filtragem da metade da esfera
        for (float temp_t : {t1, t2}) {
            if (temp_t > EPS) {
                // P(t) = P_0 + t * dr
                Ponto P = ray.P_0 + (ray.dr * temp_t);
                // guarda coomo t válidos somente da parte superior da esfera
                if (produto_escalar(P - centro, direcao) >= 0) {
                    ts_validos.push_back(temp_t);
                }
            }
        }
    }

    // base da semiesfera
    if (temBase) {
        Vetor n_base = -direcao; 
        float denom = produto_escalar(ray.dr, n_base);
        if (std::abs(denom) > 1e-6f) {
            float tb = produto_escalar(centro - ray.P_0, n_base) / denom;
            if (tb > EPS) {
                Ponto P_base = ray.P_0 + (ray.dr * tb);
                if (comprimento(P_base - centro) <= raio) {
                    ts_validos.push_back(tb);
                }
            }
        }
    }

    if (ts_validos.empty()) return false;
    t = *std::min_element(ts_validos.begin(), ts_validos.end());
    return true;
}

// calculando normal
Vetor SemiEsfera::calcularNormal(const Ponto& p) const {
    const float EPS = 1e-3f;
    Vetor v = p - centro;

    if (temBase && std::abs(produto_escalar(v, direcao)) < EPS) {
        return -direcao; 
    }

    return normalizar(v);
}


// ========== MATRIZES DE TRANSFORMAÇÕES ==========

void SemiEsfera::transforma(const Matriz4x4& M) {
    Ponto p0(0, 0, 0);
    Ponto p1(1, 0, 0);
    Ponto p0_t = M * p0;
    Ponto p1_t = M * p1;
    
    float s = comprimento(p1_t - p0_t);

    if (std::abs(s - 1.0f) > 1e-5f) {
        raio *= s;
    }

    centro = M * centro;

    direcao = normalizar(M * direcao);
}

void SemiEsfera::transladar(float tx, float ty, float tz) {
    transforma(Transformacao::translacao(tx, ty, tz));
}

void SemiEsfera::escalar(float s, Ponto ponto_fixo) {
    Matriz4x4 S = Transformacao::escala(s, s, s, ponto_fixo);
    this->centro = S * this->centro;
    this->raio *= s;
}

void SemiEsfera::rotacionarX(float anguloGraus) {
    float rad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 T1 = Transformacao::translacao(-centro.x, -centro.y, -centro.z);
    Matriz4x4 R  = Transformacao::rotacaoX(rad);
    Matriz4x4 T2 = Transformacao::translacao(centro.x, centro.y, centro.z);
    transforma(T2 * R * T1);
}

void SemiEsfera::rotacionarY(float anguloGraus) {
    float rad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 T1 = Transformacao::translacao(-centro.x, -centro.y, -centro.z);
    Matriz4x4 R  = Transformacao::rotacaoY(rad);
    Matriz4x4 T2 = Transformacao::translacao(centro.x, centro.y, centro.z);
    transforma(T2 * R * T1);
}

void SemiEsfera::rotacionarZ(float anguloGraus) {
    float rad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 T1 = Transformacao::translacao(-centro.x, -centro.y, -centro.z);
    Matriz4x4 R  = Transformacao::rotacaoZ(rad);
    Matriz4x4 T2 = Transformacao::translacao(centro.x, centro.y, centro.z);
    transforma(T2 * R * T1);
}

void SemiEsfera::rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus, Ponto ponto) {
    float rad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 R = Transformacao::rotacaoEixoArbitrarioPonto(eixo, rad, ponto);
    transforma(R);
}

void SemiEsfera::espelharXY() { 
    transforma(Transformacao::espelhamentoXY()); 
}
void SemiEsfera::espelharXZ() { 
    transforma(Transformacao::espelhamentoXZ()); 
}
void SemiEsfera::espelharYZ() { 
    transforma(Transformacao::espelhamentoYZ()); 
}