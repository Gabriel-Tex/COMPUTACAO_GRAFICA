#include "objetos/Esfera.h"
#include <cmath>
#include <iostream>

int Esfera::nextId = 0;

Esfera::Esfera(float raio, Ponto centro, Cor cor, Propriedades propriedades, int m)
    : rEsfera(raio), cEsfera(centro), cor(cor), 
      propriedades_esfera(propriedades), m_esfera(m),
      textura(nullptr), temTexturaFlag(false) {
    id = nextId++;
}

bool Esfera::intersecta(const Ray& ray, float& ti) const {
    Vetor w = ray.P_0 - cEsfera;
    
    float a = produto_escalar(ray.dr, ray.dr);
    float b = 2 * produto_escalar(w, ray.dr);
    float c = produto_escalar(w, w) - rEsfera * rEsfera;
    
    double delta = b*b - 4*a*c;
    
    if(delta < 0) return false;
    
    float t1 = (-b + sqrt(delta)) / (2 * a);
    float t2 = (-b - sqrt(delta)) / (2 * a);
    
    if (t1 > 0 && t2 > 0) ti = std::min(t1, t2);
    else if (t1 > 0) ti = t1;
    else if (t2 > 0) ti = t2;
    else return false;
    
    return true;
}

Vetor Esfera::calcularNormal(const Ponto& ponto) const {
    return normalizar(ponto - cEsfera);
}

Propriedades Esfera::getPropriedades() const {
    return propriedades_esfera;
}

int Esfera::getMaterial() const {
    return m_esfera;
}

std::string Esfera::getNome() const {
    return "Esfera";
}

int Esfera::getId() const {
    return id;
}

Cor Esfera::getCorTextura(const Ponto& ponto) const {
    if (temTexturaFlag && textura != nullptr) {
        Vetor normal = calcularNormal(ponto);
        
        float u = 0.5f + (std::atan2(normal.z, normal.x) / (2 * M_PI));
        float v = 0.5f - (std::asin(normal.y) / M_PI);
        
        return textura->amostrar(u, v);
    }
    return cor; 
}

bool Esfera::temTextura() const {
    return temTexturaFlag;
}

void Esfera::setTextura(Textura* tex) {
    textura = tex;
    temTexturaFlag = (tex != nullptr);
}