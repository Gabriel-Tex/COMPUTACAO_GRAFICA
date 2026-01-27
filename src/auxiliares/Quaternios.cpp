#include "auxiliares/Quaternios.h"
#include "auxiliares/Matrizes.h"
#include "auxiliares/Vetores.h"

Quaternio::Quaternio(float w, float x, float y, float z) 
    : w(w), x(x), y(y), z(z) {}

Quaternio Quaternio::operator*(const Quaternio& q) const {
    return Quaternio(
        w*q.w - x*q.x - y*q.y - z*q.z,
        w*q.x + x*q.w + y*q.z - z*q.y, 
        w*q.y - x*q.z + y*q.w + z*q.x, 
        w*q.z + x*q.y - y*q.x + z*q.w
    );
}

Quaternio Quaternio::conjugado() const {
    return Quaternio(w, -x, -y, -z);
}

float Quaternio::norma() const {
    return sqrt(w*w + x*x + y*y + z*z);
}

Quaternio Quaternio::normaliza() const {
    float n = norma();
    if (n == 0.0f) return Quaternio(1, 0, 0, 0);
    return Quaternio(w/n, x/n, y/n, z/n);
}

Matriz4x4 Quaternio::paraMatriz() const {
    Matriz4x4 M;
    
    Quaternio q = normaliza();
    
    float xx = q.x * q.x;
    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float xw = q.x * q.w;
    
    float yy = q.y * q.y;
    float yz = q.y * q.z;
    float yw = q.y * q.w;
    
    float zz = q.z * q.z;
    float zw = q.z * q.w;
    
    M.m[0][0] = 1.0f - 2.0f * (yy + zz);
    M.m[0][1] = 2.0f * (xy - zw);
    M.m[0][2] = 2.0f * (xz + yw);
    M.m[0][3] = 0.0f;
    
    M.m[1][0] = 2.0f * (xy + zw);
    M.m[1][1] = 1.0f - 2.0f * (xx + zz);
    M.m[1][2] = 2.0f * (yz - xw);
    M.m[1][3] = 0.0f;
    
    M.m[2][0] = 2.0f * (xz - yw);
    M.m[2][1] = 2.0f * (yz + xw);
    M.m[2][2] = 1.0f - 2.0f * (xx + yy);
    M.m[2][3] = 0.0f;
    
    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = 0.0f;
    M.m[3][3] = 1.0f;
    
    return M;
}

Quaternio Quaternio::criarQuaternio(const Vetor& eixo, float angulo) {
    Vetor e = normalizar(eixo);
    float halfAngle = angulo * 0.5f;
    float s = sin(halfAngle);
    
    return Quaternio(
        cos(halfAngle),
        e.x * s,
        e.y * s,
        e.z * s
    ).normaliza();
}