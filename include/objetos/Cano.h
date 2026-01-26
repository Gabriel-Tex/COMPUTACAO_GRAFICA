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
     
    Cano(Ponto baseCentral, float raioCorpo, float alturaCorpo, 
         float raioBorda, float alturaBorda, Vetor direcao, 
         Propriedades prop, int m);

    virtual std::string getNome() const override { return "CanoMario"; }
};

#endif