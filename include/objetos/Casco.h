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
};

#endif