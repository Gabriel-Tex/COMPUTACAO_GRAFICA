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
    
    Cogumelo(Ponto centroConexao, float raioCaule, float raioChapeu, int m);

    virtual std::string getNome() const override { return "Cogumelo"; }
};

#endif