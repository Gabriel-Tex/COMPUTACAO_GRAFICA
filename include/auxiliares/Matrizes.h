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
    Matriz4x4 rotacaoEixoArbitrario(const Vetor& eixo, float angulo);
    
    // 1.4.3. Escala
    Matriz4x4 escala(float sx, float sy, float sz);
    Matriz4x4 escalaUniforme(float s);
    
    // 1.4.4. Cisalhamento
    Matriz4x4 cisalhamentoXY(float shx, float shy);
    Matriz4x4 cisalhamentoXZ(float shx, float shz);
    Matriz4x4 cisalhamentoYZ(float shy, float shz);
    
    // 1.4.5. Espelho em relação a planos
    Matriz4x4 espelhoXY();
    Matriz4x4 espelhoXZ();
    Matriz4x4 espelhoYZ();
    
    float grausParaRadianos(float graus);
    float radianosParaGraus(float radianos);
}

#endif 