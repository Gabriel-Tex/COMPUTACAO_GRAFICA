// Intersecao.h
#ifndef INTERSECAO_H
#define INTERSECAO_H

#include "Objeto.h"
#include "Ray.h"

struct Intersecao {
    float t = std::numeric_limits<float>::max();
    Objeto* objeto = nullptr;
    Ponto ponto;
    
    Intersecao() = default;
    
    bool valida() const {
        return objeto != nullptr && t > 0;
    }
    
    void reset() {
        t = std::numeric_limits<float>::max();
        objeto = nullptr;
    }
};

#endif