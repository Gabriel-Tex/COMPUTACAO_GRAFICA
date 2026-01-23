#ifndef OBJETO_H
#define OBJETO_H

#include "./auxiliares/Ray.h"
#include "./auxiliares/Pontos.h"
#include "./auxiliares/Vetores.h"
#include "./auxiliares/Iluminacao.h"
#include "./auxiliares/Textura.h"
#include "./auxiliares/Matrizes.h"
#include <string>

class Matriz4x4;

class Objeto {
public:
    virtual ~Objeto() = default;
    
    virtual bool intersecta(const Ray& ray, float& t) const = 0;
    virtual Vetor calcularNormal(const Ponto& ponto) const = 0;
    virtual Propriedades getPropriedades() const = 0;
    virtual int getMaterial() const = 0;
    virtual std::string getNome() const = 0;
    virtual int getId() const = 0;
    
    virtual Cor getCorTextura(const Ponto& ponto) const {
        return getPropriedades().Kdif;
    }
    
    virtual bool temTextura() const { return false; }
    virtual void setTextura(Textura* tex) { (void)tex; }
    
    virtual bool verificarIntersecaoSombra(const Ray& shadowRay, float distanciaLuz) const {
        float t;
        return intersecta(shadowRay, t) && t > 0 && t < distanciaLuz;
    }

    virtual void transforma(const Matriz4x4& M) = 0;
};

#endif