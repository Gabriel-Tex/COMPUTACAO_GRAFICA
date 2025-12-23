#ifndef PONTOS_H
#define PONTOS_H

// ============== DEFINIÇÃO DA CLASSE ==============

class Ponto
{
public:
    float x, y, z;
    Ponto() : x(0.0f), y(0.0f), z(0.0f) {}
    Ponto(float x, float y, float z);
};

// ============== MÉTODOS ASSOCIADOS A PONTOS ==============

#endif