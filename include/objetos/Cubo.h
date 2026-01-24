#ifndef CUBO_H
#define CUBO_H

#include "Objeto.h"
#include "ListMesh.h"
#include "Triangulo.h"
#include "./auxiliares/Matrizes.h"
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
    int id;
    
public:
    Cubo(Ponto centro, float lado, Propriedades prop, int m);
    
    bool intersecta(const Ray& ray, float& t) const override;
    Vetor calcularNormal(const Ponto& ponto) const override;
    Propriedades getPropriedades() const override;
    int getMaterial() const override;
    std::string getNome() const override;
    int getId() const override;
    
    Cor getCorTextura(const Ponto& ponto) const override;
    bool temTextura() const override;
    void setTextura(Textura* tex) override;
    
    Ponto getCentro() const { return centro; }
    float getLado() const { return lado; }
    
    Vetor normalNoPonto(Ponto p) const;

    void transforma(const Matriz4x4& M);
     void transladar(float tx, float ty, float tz);
    void escalar(float s, Ponto ponto_fixo = Ponto(0,0,0));
    void rotacionarX(float angulo);
    void rotacionarY(float angulo);
    void rotacionarZ(float angulo);
    void cisalharX_XZ(float angulo);
    void cisalharY_XY(float angulo);
    void cisalharY_XZ(float angulo);
    void cisalharZ_XY(float angulo);
    void espelharXY();
    void espelharXZ();
    void espelharYZ();
    
private:
    void inicializarMesh();
};

#endif