#ifndef ESFERA_H
#define ESFERA_H

#include "Objeto.h"
#include "./auxiliares/Textura.h"
#include <string>

#define M_PI 3.14159265358979323846

class Esfera : public Objeto {
private:
    float rEsfera;
    Ponto cEsfera;
    Cor cor;  // Cor base (do seu construtor original)
    Propriedades propriedades_esfera;
    int m_esfera;
    Textura* textura;
    bool temTexturaFlag;
    static int nextId;
    int id;
    
public:
    // Construtor mantendo compatibilidade com seu código
    Esfera(float raio, Ponto centro, Cor cor, Propriedades propriedades, int m);
    
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
    
    // Getters específicos (para compatibilidade)
    float getRaio() const { return rEsfera; }
    Ponto getCentro() const { return cEsfera; }
    Cor getCor() const { return cor; }
    
    // Setters
    void setCor(const Cor& novaCor) { cor = novaCor; }
};

#endif