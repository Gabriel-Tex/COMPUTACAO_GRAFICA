#include "objetos/ObjetoComplexo.h"
#include "objetos/Esfera.h"
#include "objetos/Cilindro.h"
#include "objetos/Cone.h"
#include "objetos/Cubo.h"
#include <limits>

ObjetoComplexo::ObjetoComplexo() 
    : componenteAtingido(nullptr) {
}

bool ObjetoComplexo::intersecta(const Ray& ray, float& ti) const {
    float t_min = numeric_limits<float>::max();
    componenteAtingido = nullptr;
    
    for (auto comp : componentes) {
        float t_temp;
        if (comp->intersecta(ray, t_temp)) {
            if (t_temp > 1e-6f && t_temp < t_min) {
                t_min = t_temp;
                componenteAtingido = comp;
            }
        }
    }
    
    if (componenteAtingido) {
        ti = t_min;
        return true;
    }
    
    return false;
}

Vetor ObjetoComplexo::calcularNormal(const Ponto& ponto) const {
    if (!componenteAtingido) return Vetor(0, 1, 0);
    return componenteAtingido->calcularNormal(ponto);
}

string ObjetoComplexo::getNome() const {
    return "ObjetoComplexo";
}

int ObjetoComplexo::getId() const {
    return -1;
}

Cor ObjetoComplexo::getCorTextura(const Ponto& ponto) const {
    if (!componenteAtingido) return Cor(1, 1, 1);
    return componenteAtingido->getCorTextura(ponto);
}

bool ObjetoComplexo::temTextura() const {
    if (!componenteAtingido) return false;
    return componenteAtingido->temTextura();
}

void ObjetoComplexo::setTextura(Textura* tex) {
    for (auto comp : componentes) {
        comp->setTextura(tex);
    }
}

void ObjetoComplexo::adicionarComponente(unique_ptr<Objeto> obj) {
    componentes.push_back(obj.get());
    componentesOwned.push_back(move(obj));
}

