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
};

#endif