#ifndef PLANO_H
#define PLANO_H

#include "Objeto.h"
#include "./auxiliares/Matrizes.h"
#include "./auxiliares/Textura.h"
#include <string>

class Plano : public Objeto {
private:
    Ponto P_pi;
    Vetor n;
    Propriedades prop;
    int m;
    Textura* textura;
    bool temTexturaFlag;
    int id;
    
public:
    Plano(Ponto P, Vetor N, Propriedades propriedades, int m);
    
    bool intersecta(const Ray& ray, float& ti) const override;
    Vetor calcularNormal(const Ponto& ponto) const override;
    Propriedades getPropriedades() const override;
    int getMaterial() const override;
    std::string getNome() const override;
    int getId() const override;
    
    Cor getCorTextura(const Ponto& ponto) const override;
    bool temTextura() const override;
    void setTextura(Textura* tex) override;
    
    Ponto getPonto() const { return P_pi; }
    Vetor getNormal() const { return n; }
    
    Cor getCorTexturaPt(const Ponto& Pt) const;

    void transforma(const Matriz4x4& M);
  
};

#endif