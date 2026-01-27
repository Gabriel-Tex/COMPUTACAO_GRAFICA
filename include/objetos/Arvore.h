#ifndef ARVORE_H
#define ARVORE_H

#include "ObjetoComplexo.h"
#include "Cilindro.h"
#include "SemiEsfera.h"

class Arvore : public ObjetoComplexo {
public:
    
    Ponto baseTronco;
    float alturaTronco;
    float raioTronco;
    float raioCopa;
    int m;
    
    Arvore(Ponto baseTronco, float alturaTronco, float raioTronco, float raioCopa, int m);

    virtual std::string getNome() const override { return "Arvore"; }

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