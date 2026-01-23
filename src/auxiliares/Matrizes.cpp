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
Matriz4x4 Transformacao::rotacaoEixoArbitrario(const Vetor& eixo, float angulo) {
    Vetor u = normalizar(eixo);
    float ux = u.x, uy = u.y, uz = u.z;
    float c = cos(angulo);
    float s = sin(angulo);
    float omc = 1.0f - c;
    
    Matriz4x4 R;
    
    R.m[0][0] = c + ux*ux*omc;      
    R.m[0][1] = ux*uy*omc - uz*s;  
    R.m[0][2] = ux*uz*omc + uy*s;
    
    R.m[1][0] = uy*ux*omc + uz*s;   
    R.m[1][1] = c + uy*uy*omc;      
    R.m[1][2] = uy*uz*omc - ux*s;
    
    R.m[2][0] = uz*ux*omc - uy*s;   
    R.m[2][1] = uz*uy*omc + ux*s;  
    R.m[2][2] = c + uz*uz*omc;
    
    return R;
}

// 1.4.3. Escala
Matriz4x4 Transformacao::escala(float sx, float sy, float sz) {
    Matriz4x4 S;
    S.m[0][0] = sx;
    S.m[1][1] = sy;
    S.m[2][2] = sz;
    return S;
}

Matriz4x4 Transformacao::escalaUniforme(float s) {
    return escala(s, s, s);
}

// 1.4.4. Cisalhamento
Matriz4x4 Transformacao::cisalhamentoXY(float shx, float shy) {
    Matriz4x4 SH;
    SH.m[0][2] = shx;
    SH.m[1][2] = shy;
    return SH;
}

Matriz4x4 Transformacao::cisalhamentoXZ(float shx, float shz) {
    Matriz4x4 SH;
    SH.m[0][1] = shx;
    SH.m[2][1] = shz;
    return SH;
}

Matriz4x4 Transformacao::cisalhamentoYZ(float shy, float shz) {
    Matriz4x4 SH;
    SH.m[1][0] = shy;
    SH.m[2][0] = shz;
    return SH;
}

// 1.4.5. Espelho em relação a planos
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

// Conversão de ângulos
float Transformacao::grausParaRadianos(float graus) {
    return graus * M_PI / 180.0f;
}

float Transformacao::radianosParaGraus(float radianos) {
    return radianos * 180.0f / M_PI;
}