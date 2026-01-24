#ifndef MATRIZES_H
#define MATRIZES_H

#include "Pontos.h"
#include "Vetores.h"
#include <cmath>

#define M_PI 3.14159265358979323846
// Classe para matrizes 4x4
class Matriz4x4 {
public:
    float m[4][4];
    
    // Construtor
    Matriz4x4();
    
    // Multiplicação de matrizes
    Matriz4x4 operator*(const Matriz4x4& outra) const;
    
    // Multiplicação de matriz por ponto
    Ponto operator*(const Ponto& p) const;
    
    // Multiplicação de matriz por vetor (ignora translação)
    Vetor operator*(const Vetor& v) const;
};

namespace Transformacao {
    // 1.4.1. Translação
    Matriz4x4 translacao(float tx, float ty, float tz);
    Matriz4x4 translacao(const Vetor& t);
    
    // 1.4.2. Rotação em torno dos eixos principais
    Matriz4x4 rotacaoX(float angulo);   
    Matriz4x4 rotacaoY(float angulo);    
    Matriz4x4 rotacaoZ(float angulo);    
    
    // 1.4.2. Rotação em torno de eixo arbitrário
    
    // 1.4.3. Escala
    Matriz4x4 escalaOrigem(float sx, float sy, float sz);
    Matriz4x4 escala(float sx, float sy, float sz, Ponto ponto_fixo);
    
    // 1.4.4. Cisalhamento
    Matriz4x4 cisalhamentoX_XZ(float anguloGraus);
    Matriz4x4 cisalhamentoY_XZ(float anguloGraus);
    Matriz4x4 cisalhamentoY_XY(float anguloGraus);
    Matriz4x4 cisalhamentoZ_XY(float anguloGraus);
    
    // 1.4.5. Espelho 
    Matriz4x4 espelhoXY();
    Matriz4x4 espelhoXZ();
    Matriz4x4 espelhoYZ();
    
    float grausParaRadianos(float graus);
}

#endif 