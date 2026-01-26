#include "objetos/SemiEsfera.h"
#include <cmath>

SemiEsfera::SemiEsfera(Ponto centro, float raio, Vetor direcao, Propriedades props, int materialId, int m, bool temBase)
    : centro(centro), raio(raio), direcao(normalizar(direcao)), 
      props(props), m(m), temBase(temBase), 
      textura(nullptr), temTexturaFlag(false) {}

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

        for (float temp_t : {t1, t2}) {
            if (temp_t > EPS) {
                Ponto P = ray.P_0 + (ray.dr * temp_t);
                if (produto_escalar(P - centro, direcao) >= 0) {
                    ts_validos.push_back(temp_t);
                }
            }
        }
    }

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

Vetor SemiEsfera::calcularNormal(const Ponto& p) const {
    const float EPS = 1e-3f;
    Vetor v = p - centro;

    if (temBase && std::abs(produto_escalar(v, direcao)) < EPS) {
        return -direcao; 
    }

    return normalizar(v);
}

void SemiEsfera::setTextura(Textura* tex) {
    textura = tex;
    temTexturaFlag = (tex != nullptr);
}

Cor SemiEsfera::getCorTextura(const Ponto& ponto) const {
    if (temTexturaFlag && textura != nullptr) {
        Vetor n = normalizar(ponto - centro);
        float u = 0.5f + (atan2(n.z, n.x) / (2.0f * M_PI));
        float v = 0.5f - (asin(n.y) / M_PI);
        return textura->amostrar(u, v);
    }
    return props.Kdif;
}

// ========== TRANSFORMACÕES ==========

void SemiEsfera::transforma(const Matriz4x4& M) {
    centro = M * centro;
    direcao = normalizar(M * direcao);
}
