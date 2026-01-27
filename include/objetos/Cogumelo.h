#ifndef COGUMELO_H
#define COGUMELO_H

#include "ObjetoComplexo.h"
#include "SemiEsfera.h"
#include "./auxiliares/Iluminacao.h"

class Cogumelo : public ObjetoComplexo {
public:
    
    Ponto centroConexao;
    float raioCaule;
    float raioChapeu;
    float m;
    Ponto centroBase;
    
    Cogumelo(Ponto centroConexao, float raioCaule, float raioChapeu, int m);

    virtual std::string getNome() const override { return "Cogumelo"; }

    void transforma(const Matriz4x4& M);
    void transladar(float tx, float ty, float tz);
    void espelharXY();
    void espelharXZ();
    void espelharYZ();
    void rotacionarX(float anguloGraus);
    void rotacionarY(float anguloGraus);
    void rotacionarZ(float anguloGraus);

    void rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus);

    void escalar(float s);
};

#endif