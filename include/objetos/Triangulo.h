#ifndef TRIANGULO_H
#define TRIANGULO_H

#include "./auxiliares/Pontos.h"
#include "./auxiliares/Vetores.h"
#include "./auxiliares/Iluminacao.h"
#include "./auxiliares/Matrizes.h"
#include "Objeto.h"
#include <string>

// ============== DEFINIÇÃO DA CLASSE ==============

class Triangulo : public Objeto { 
public:
    Ponto A, B, C;
    Vetor n;
    Propriedades prop;
    int m;
    int id;

    Triangulo(Ponto A, Ponto B, Ponto C, Vetor n, Propriedades prop, int m);

    virtual bool intersecta(const Ray& ray, float& t) const override;
    virtual Vetor calcularNormal(const Ponto& p) const override;
    virtual Propriedades getPropriedades() const override { return prop; }
    virtual int getMaterial() const override { return m; }
    virtual std::string getNome() const override { return "Triangulo"; }
    virtual int getId() const override { return 0; }
    
    virtual void transforma(const Matriz4x4& M) override;
};

// ============== MÉTODOS ASSOCIADOS AO TRIÂNGULO ==============

bool IntersecaoRayTriangulo(Triangulo triangulo, Ray ray, float &ti);


#endif