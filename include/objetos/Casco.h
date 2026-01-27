#ifndef CASCO_H
#define CASCO_H

#include "ObjetoComplexo.h"
#include "SemiEsfera.h"
#include "Cone.h"

class Casco : public ObjetoComplexo {
public:
    Ponto centroBase;
    float raioCasco;
    int m;

    Casco(Ponto centroBase, float raioCasco, int m);

    virtual std::string getNome() const override { return "Casco"; }
    
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