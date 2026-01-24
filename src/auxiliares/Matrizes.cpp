#include "auxiliares/Matrizes.h"
#include <cmath>

// ============== IMPLEMENTAÇÃO DA CLASSE Matriz4x4 ==============

Matriz4x4::Matriz4x4() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

Matriz4x4 Matriz4x4::operator*(const Matriz4x4& outra) const {
    Matriz4x4 resultado;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float soma = 0.0f;
            for (int k = 0; k < 4; k++) {
                soma += m[i][k] * outra.m[k][j];
            }
            resultado.m[i][j] = soma;
        }
    }
    
    return resultado;
}

Ponto Matriz4x4::operator*(const Ponto& p) const {
    float x = m[0][0]*p.x + m[0][1]*p.y + m[0][2]*p.z + m[0][3];
    float y = m[1][0]*p.x + m[1][1]*p.y + m[1][2]*p.z + m[1][3];
    float z = m[2][0]*p.x + m[2][1]*p.y + m[2][2]*p.z + m[2][3];
    float w = m[3][0]*p.x + m[3][1]*p.y + m[3][2]*p.z + m[3][3];
    
    if (w != 1.0f && w != 0.0f) {
        return Ponto(x / w, y / w, z / w);
    }
    
    return Ponto(x, y, z);
}

Vetor Matriz4x4::operator*(const Vetor& v) const {
    float x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z;
    float y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z;
    float z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z;
    
    return Vetor(x, y, z);
}

// ============== IMPLEMENTAÇÃO DAS FUNÇÕES DE TRANSFORMAÇÃO ==============

// 1.4.1. Translação
Matriz4x4 Transformacao::translacao(float tx, float ty, float tz) {
    Matriz4x4 T;
    T.m[0][3] = tx;
    T.m[1][3] = ty; 
    T.m[2][3] = tz;
    return T;
}

Matriz4x4 Transformacao::translacao(const Vetor& t) {
    return translacao(t.x, t.y, t.z);
}

// 1.4.2. Rotação em torno dos eixos principais
Matriz4x4 Transformacao::rotacaoX(float angulo) {
    float c = cos(angulo);
    float s = sin(angulo);
    Matriz4x4 R;
    R.m[1][1] = c; 
    R.m[1][2] = -s;
    R.m[2][1] = s; 
    R.m[2][2] = c;
    return R;
}

Matriz4x4 Transformacao::rotacaoY(float angulo) {
    float c = cos(angulo);
    float s = sin(angulo);
    Matriz4x4 R;
    R.m[0][0] = c;  
    R.m[0][2] = s;
    R.m[2][0] = -s; 
    R.m[2][2] = c;
    return R;
}

Matriz4x4 Transformacao::rotacaoZ(float angulo) {
    float c = cos(angulo);
    float s = sin(angulo);
    Matriz4x4 R;
    R.m[0][0] = c; 
    R.m[0][1] = -s;
    R.m[1][0] = s; 
    R.m[1][1] = c;
    return R;
}

// 1.4.2. Rotação em torno de eixo arbitrário

// 1.4.3. Escala
Matriz4x4 Transformacao::escalaOrigem(float sx, float sy, float sz) {

    Matriz4x4 S;
    S.m[0][0] = sx;
    S.m[1][1] = sy;
    S.m[2][2] = sz;

    return S;
}

Matriz4x4 Transformacao::escala(float sx, float sy, float sz, Ponto ponto_fixo) {
    Matriz4x4 T1 = Transformacao::translacao(-ponto_fixo.x, -ponto_fixo.y, -ponto_fixo.z);    
    Matriz4x4 S = Transformacao::escalaOrigem(sx, sy, sz);
    Matriz4x4 T2 = Transformacao::translacao(ponto_fixo.x, ponto_fixo.y, ponto_fixo.z);
    Matriz4x4 M = T2 * S * T1;
    
    return M;
}

// 1.4.4. Cisalhamento
Matriz4x4 Transformacao::cisalhamentoX_XZ(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    
    Matriz4x4 C;
    C.m[0][1] = tan(anguloRad);
    return C;
}

Matriz4x4 Transformacao::cisalhamentoY_XZ(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    
    Matriz4x4 C;
    C.m[1][0] = tan(anguloRad);
    return C;
}

Matriz4x4 Transformacao::cisalhamentoY_XY(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    
    Matriz4x4 C;
    C.m[1][2] = tan(anguloRad);
    return C;
}

Matriz4x4 Transformacao::cisalhamentoZ_XY(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    
    Matriz4x4 C;
    C.m[2][1] = tan(anguloRad);
    return C;
}

// 1.4.5. Espelho 
Matriz4x4 Transformacao::espelhoXY() {
    Matriz4x4 M;
    M.m[2][2] = -1.0f;
    return M;
}

Matriz4x4 Transformacao::espelhoXZ() {
    Matriz4x4 M;
    M.m[1][1] = -1.0f;
    return M;
}

Matriz4x4 Transformacao::espelhoYZ() {
    Matriz4x4 M;
    M.m[0][0] = -1.0f;
    return M;
}

float Transformacao::grausParaRadianos(float graus) {
    return graus * M_PI / 180.0f;
}
