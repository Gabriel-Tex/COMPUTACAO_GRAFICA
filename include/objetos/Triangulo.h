#ifndef TRIANGULO_H
#define TRIANGULO_H

#include "./auxiliares/Pontos.h"
#include "./auxiliares/Vetores.h"
#include "./auxiliares/Iluminacao.h"

// ============== DEFINIÇÃO DA CLASSE ==============

class Triangulo
{
public:
    Ponto A, B, C;
    Vetor n;
    Propriedades prop;
    int m;

    Triangulo(Ponto A, Ponto B, Ponto C, Vetor n, Propriedades prop, int m);

private:
    // Função auxiliar para cálculo de coordenadas baricêntricas
    bool estaDentroTriangulo(const Ponto& P) const;
};

// ============== MÉTODOS ASSOCIADOS AO TRIÂNGULO ==============

bool IntersecaoRayTriangulo(Triangulo triangulo, Ray ray, float &ti);

void normalTriangulo(Triangulo triangulo);

#endif