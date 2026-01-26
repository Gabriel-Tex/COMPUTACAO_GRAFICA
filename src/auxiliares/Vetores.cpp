#include "auxiliares/Vetores.h"
// ============== CLASSE ==============

// definição de um vetor no R3
Vetor::Vetor(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

// ============== MÉTODOS ASSOCIADOS A VETORES ==============
/*
PF -> ponto final
PI -> ponto inicial
PF - PI = VETOR na direção PI -> PF
*/
Vetor operator-(const Ponto &PF, const Ponto &PI){
    return Vetor(PF.x - PI.x, PF.y - PI.y, PF.z - PI.z);
}

Vetor operator-(const Vetor &v, const Vetor &u){
    return Vetor(v.x - u.x, v.y - u.y, v.z - u.z);
}

// inverter sentido de vetor 
Vetor operator-(const Vetor &V){
    return Vetor(-V.x, -V.y, -V.z);
}

Vetor operator*(const Vetor &V, const float &u){
    return Vetor(V.x * u, V.y * u, V.z * u);
}

Vetor operator*(const float &u, const Vetor &V){
    return Vetor(V.x * u, V.y * u, V.z * u);
}

Ponto operator+(const Vetor &V, const Ponto &U){
    return Ponto(V.x + U.x, V.y + U.y, V.z + U.z);
}

Ponto operator+(const Ponto &U, const Vetor &V){
    return Ponto(V.x + U.x, V.y + U.y, V.z + U.z);
}

Vetor operator+(const Vetor &V, const Vetor &U){
    return Vetor(V.x + U.x, V.y + U.y, V.z + U.z);
}

// produto escalar entre dois vetores
float produto_escalar(Vetor V1, Vetor V2){
    return  V1.x * V2.x + 
            V1.y * V2.y + 
            V1.z * V2.z;
}

// produto vetorial entre dois vetores
Vetor produto_vetorial(Vetor V1, Vetor V2){
    return Vetor(V1.y * V2.z - V1.z * V2.y,
                 V1.z * V2.x - V1.x * V2.z,
                 V1.x * V2.y - V1.y * V2.x);
}

// ||v|| = sqrt(x^2 + y^2 + z^2)
// normalização: v/||v||
Vetor normalizar(Vetor V){
    // raiz do produto escalar entre o vetor e ele mesmo
    float modulo = sqrtf(V.x * V.x + V.y * V.y + V.z * V.z);

    return Vetor(V.x / modulo, V.y / modulo, V.z / modulo);
}

float comprimento(Vetor V){
    return sqrt(V.x * V.x + V.y * V.y + V.z * V.z);
}