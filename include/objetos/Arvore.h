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
};

#endif