#include "auxiliares/Pontos.h"

// ============== CLASSE ==============

// definição de um ponto no R3
Ponto::Ponto(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

// ============== MÉTODOS ASSOCIADOS A PONTOS ==============

Ponto operator+(const Ponto &PF, const Ponto &PI){
    return Ponto(PF.x + PI.x, PF.y + PI.y, PF.z + PI.z);
}

Ponto operator*(const Ponto &PF, const float &n){
    return Ponto(PF.x * n, PF.y * n, PF.z * n);
}