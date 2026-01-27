#ifndef BANDEIRA_H
#define BANDEIRA_H

#include "ObjetoComplexo.h"
#include "Cilindro.h"
#include "Esfera.h"
#include "Triangulo.h"

class Bandeira : public ObjetoComplexo {
public:
    Ponto baseHaste;
    float alturaHaste;
    int m;

    Bandeira(Ponto baseHaste, float alturaHaste, int m);

    virtual std::string getNome() const override { return "Bandeira"; }

    void transforma(const Matriz4x4& M);
    void transladar(float tx, float ty, float tz);
    void escalar(float s);
    void rotacionarX(float anguloGraus);
    void rotacionarY(float anguloGraus);
    void rotacionarZ(float anguloGraus);
    void rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus);
    void espelharXY();
    void espelharXZ();
    void espelharYZ();
};

#endif