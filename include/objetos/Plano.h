#ifndef PLANO_H
#define PLANO_H

#include "Objeto.h"
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
    static int nextId;
    int id;
    
public:
    // Construtor mantendo compatibilidade
    Plano(Ponto P, Vetor N, Propriedades propriedades, int m);
    
    // Métodos da interface Objeto
    bool intersecta(const Ray& ray, float& ti) const override;
    Vetor calcularNormal(const Ponto& ponto) const override;
    Propriedades getPropriedades() const override;
    int getMaterial() const override;
    std::string getNome() const override;
    int getId() const override;
    
    // Métodos de textura
    Cor getCorTextura(const Ponto& ponto) const override;
    bool temTextura() const override;
    void setTextura(Textura* tex) override;
    
    // Getters específicos
    Ponto getPonto() const { return P_pi; }
    Vetor getNormal() const { return n; }
    
    // Método para compatibilidade com seu código existente
    Cor getCorTexturaPt(const Ponto& Pt) const;
};

#endif