#ifndef CANO_H
#define CANO_H

#include "ObjetoComplexo.h"
#include "Cilindro.h"

class Cano : public ObjetoComplexo {
public:
    
    Ponto baseCentral; 
    float raioCorpo; 
    float alturaCorpo; 
    float raioBorda; 
    float alturaBorda; 
    Vetor direcao; 
    Propriedades prop;
    Vetor eixoArmazenado; 
    float alturaArmazenada; 
     
    Cano(Ponto baseCentral, float raioCorpo, float alturaCorpo, 
         float raioBorda, float alturaBorda, Vetor direcao, 
         Propriedades prop, int m);

    virtual std::string getNome() const override { return "CanoMario"; }

    void transforma(const Matriz4x4& M) override;

    void transladar(float tx, float ty, float tz);
    void escalar(float s);
    
    void rotacionarX(float anguloGraus);
    void rotacionarY(float anguloGraus);
    void rotacionarZ(float anguloGraus);
    void rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus);

    void cisalharX_XZ(float anguloGraus);
    void cisalharY_XZ(float anguloGraus);
    void cisalharY_XY(float anguloGraus);
    void cisalharZ_XY(float anguloGraus);

    void espelharXY();
    void espelharXZ();
    void espelharYZ();
};

#endif