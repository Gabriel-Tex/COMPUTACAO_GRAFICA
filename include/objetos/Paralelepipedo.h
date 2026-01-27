#ifndef PARALELEPIPEDO_H
#define PARALELEPIPEDO_H

#include "Objeto.h"
#include "Triangulo.h"
#include "ListMesh.h"

class Paralelepipedo : public Objeto {
private:
    Ponto centro;
    float largura, altura, comprimento;
    Propriedades prop;
    int m;
    int id;

    ListMesh mesh;
    Textura* textura;
    bool temTexturaFlag;
    float escalaTextura;
    mutable Triangulo* ultimoTrianguloAtingido;

    void inicializarMesh();

public:
    Paralelepipedo(Ponto centro, float largura, float altura, float comprimento, Propriedades prop, int m, int id = 0);

    bool intersecta(const Ray& ray, float& t) const override;
    Vetor calcularNormal(const Ponto& ponto) const override;
    Propriedades getPropriedades() const override { return prop; }
    int getMaterial() const override { return m; }
    std::string getNome() const override { return "Paralelepipedo"; }
    int getId() const override { return id; }
    Ponto getCentro() const { return centro; }
    void transforma(const Matriz4x4& M) override;

    Cor getCorTextura(const Ponto& ponto) const override;
    bool temTextura() const override { return temTexturaFlag; }
    void setTextura(Textura* tex) override { textura = tex; temTexturaFlag = (tex != nullptr); }
    
    void setEscalaTextura(float s) { escalaTextura = s; }

    void transladar(float tx, float ty, float tz);
    void escalar(float sx, float sy, float sz, Ponto ponto_fixo);
    void rotacionarX(float anguloGraus);
    void rotacionarY(float anguloGraus);
    void rotacionarZ(float anguloGraus);
    void rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus, Ponto ponto);

    void cisalharX_XZ(float anguloGraus);
    void cisalharY_XZ(float anguloGraus);
    void cisalharY_XY(float anguloGraus);
    void cisalharZ_XY(float anguloGraus);

    void espelharXY();
    void espelharXZ();
    void espelharYZ();

};

#endif