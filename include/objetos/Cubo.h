#ifndef CUBO_H
#define CUBO_H

#include "Objeto.h"
#include "ListMesh.h"
#include "Triangulo.h"
#include "./auxiliares/Textura.h"
#include <string>

class Cubo : public Objeto {
private:
    Ponto centro;
    float lado;
    Propriedades prop;
    int m;
    ListMesh mesh;
    Textura* textura;
    bool temTexturaFlag;
    static int nextId;
    int id;
    
public:
    // Construtor mantendo compatibilidade
    Cubo(Ponto centro, float lado, Propriedades prop, int m);
    
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
    Ponto getCentro() const { return centro; }
    float getLado() const { return lado; }
    
    // Método para compatibilidade
    Vetor normalNoPonto(Ponto p) const;
    
private:
    void inicializarMesh();
};

#endif