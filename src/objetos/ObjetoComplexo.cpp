#include "objetos/ObjetoComplexo.h"
#include <limits>

using namespace std;

ObjetoComplexo::ObjetoComplexo() 
    : componenteAtingido(nullptr),
      props(Propriedades(Cor(1,1,1), Cor(1,1,1), Cor(0.1,0.1,0.1))),
      materialId(10) {
}

void ObjetoComplexo::adicionarComponente(Objeto* obj) {
    componentes.push_back(obj);
}

bool ObjetoComplexo::temTextura() const {
    if (componenteAtingido) {
        return componenteAtingido->temTextura();
    }
    return false;
}

bool ObjetoComplexo::intersecta(const Ray& ray, float& ti) const {
    float t_min = numeric_limits<float>::max();
    Objeto* hit_temp = nullptr;
    
    for (auto comp : componentes) {
        float t_temp;
        if (comp->intersecta(ray, t_temp)) {
            if (t_temp > 1e-4f && t_temp < t_min) {
                t_min = t_temp;
                hit_temp = comp;
            }
        }
    }
    
    if (hit_temp) {
        ti = t_min;
        componenteAtingido = hit_temp;
        return true;
    }
    return false;
}

Vetor ObjetoComplexo::calcularNormal(const Ponto& ponto) const {
    if (componenteAtingido) {
        return componenteAtingido->calcularNormal(ponto);
    }
    return Vetor(0, 1, 0);
}

Propriedades ObjetoComplexo::getPropriedades() const {
    return componenteAtingido ? componenteAtingido->getPropriedades() : props;
}

int ObjetoComplexo::getMaterial() const {
    return componenteAtingido ? componenteAtingido->getMaterial() : materialId;
}

string ObjetoComplexo::getNome() const {
    return "ObjetoComplexo";
}

int ObjetoComplexo::getId() const {
    return -1;
}

void ObjetoComplexo::setTextura(Textura* tex) {
    for (auto comp : componentes) {
        comp->setTextura(tex);
    }
}

Cor ObjetoComplexo::getCorTextura(const Ponto& ponto) const {
    if (componenteAtingido) {
        return componenteAtingido->getCorTextura(ponto);
    }
    return props.Kdif;
}

// ================= TRANSFORMACÕES =================

void ObjetoComplexo::transforma(const Matriz4x4& M) {
    for (auto comp : componentes) {
        comp->transforma(M);
    }
}

void ObjetoComplexo::transladar(float tx, float ty, float tz) {
    transforma(Transformacao::translacao(tx, ty, tz));
}

void ObjetoComplexo::escalar(float sx, float sy, float sz, Ponto ponto_fixo) {
    transforma(Transformacao::escala(sx, sy, sz, ponto_fixo));
}

void ObjetoComplexo::rotacionarX(float anguloGraus, Ponto ponto_fixo) {
    float rad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 T1 = Transformacao::translacao(-ponto_fixo.x, -ponto_fixo.y, -ponto_fixo.z);
    Matriz4x4 R  = Transformacao::rotacaoX(rad);
    Matriz4x4 T2 = Transformacao::translacao(ponto_fixo.x, ponto_fixo.y, ponto_fixo.z);
    transforma(T2 * R * T1);
}

void ObjetoComplexo::rotacionarY(float anguloGraus, Ponto ponto_fixo) {
    float rad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 T1 = Transformacao::translacao(-ponto_fixo.x, -ponto_fixo.y, -ponto_fixo.z);
    Matriz4x4 R  = Transformacao::rotacaoY(rad);
    Matriz4x4 T2 = Transformacao::translacao(ponto_fixo.x, ponto_fixo.y, ponto_fixo.z);
    transforma(T2 * R * T1);
}

void ObjetoComplexo::rotacionarZ(float anguloGraus, Ponto ponto_fixo) {
    float rad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 T1 = Transformacao::translacao(-ponto_fixo.x, -ponto_fixo.y, -ponto_fixo.z);
    Matriz4x4 R  = Transformacao::rotacaoZ(rad);
    Matriz4x4 T2 = Transformacao::translacao(ponto_fixo.x, ponto_fixo.y, ponto_fixo.z);
    transforma(T2 * R * T1);
}