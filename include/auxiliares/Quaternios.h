#ifndef QUATERNIOS_H
#define QUATERNIOS_H

#include <cmath>

class Matriz4x4;
class Vetor;
class Ponto;

class Quaternio {
public:
    float w, x, y, z;  
    
    Quaternio(float w = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f);
    
    Quaternio operator*(const Quaternio& q) const;
    Quaternio conjugado() const;
    float norma() const;
    Quaternio normaliza() const;
    
    Matriz4x4 paraMatriz() const;
    
    static Quaternio criarQuaternio(const Vetor& eixo, float angulo);
};


#endif
