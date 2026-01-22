#ifndef CONE_H
#define CONE_H

#include "Objeto.h"
#include "./auxiliares/Textura.h"
#include <string>

#define M_PI 3.14159265358979323846

class Cone : public Objeto {
private:
    Ponto CB;
    float rbCone;
    float altura;
    Vetor dco;
    Propriedades prop;
    int m;
    Textura* textura;
    bool temTexturaFlag;
    static int nextId;
    int id;
    
public:
    // Construtor mantendo compatibilidade
    Cone(Ponto CB, float rbCone, float altura, Vetor dco, Propriedades prop, int m);
    
    // Métodos da interface Objeto
    bool intersecta(const Ray& ray, float& t) const override;
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
    Ponto getBase() const { return CB; }
    float getRaioBase() const { return rbCone; }
    float getAltura() const { return altura; }
    Vetor getDirecao() const { return dco; }
    Ponto getVertice() const;
    
private:
    float calcularRazao() const;
};

#endif