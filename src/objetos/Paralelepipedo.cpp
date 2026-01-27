#include "objetos/Paralelepipedo.h"
#include <cmath>

Paralelepipedo::Paralelepipedo(Ponto centro, float largura, float altura, float comprimento, Propriedades prop, int m, int id)
    : centro(centro), largura(largura), altura(altura), comprimento(comprimento), 
      prop(prop), m(m), id(id), textura(nullptr), temTexturaFlag(false), escalaTextura(1.0f) {
    inicializarMesh();
}

void Paralelepipedo::inicializarMesh() {
    mesh.limpar(); 
    
    float dx = largura / 2.0f;
    float dy = altura / 2.0f;
    float dz = comprimento / 2.0f;

    Ponto v0(centro.x - dx, centro.y - dy, centro.z - dz);
    Ponto v1(centro.x + dx, centro.y - dy, centro.z - dz);
    Ponto v2(centro.x + dx, centro.y + dy, centro.z - dz);
    Ponto v3(centro.x - dx, centro.y + dy, centro.z - dz);
    Ponto v4(centro.x - dx, centro.y - dy, centro.z + dz);
    Ponto v5(centro.x + dx, centro.y - dy, centro.z + dz);
    Ponto v6(centro.x + dx, centro.y + dy, centro.z + dz);
    Ponto v7(centro.x - dx, centro.y + dy, centro.z + dz);

    mesh.addTriangulo(Triangulo(v4, v5, v6, Vetor(0,0,1), prop, m));  mesh.addTriangulo(Triangulo(v4, v6, v7, Vetor(0,0,1), prop, m));  // Frente
    mesh.addTriangulo(Triangulo(v0, v2, v1, Vetor(0,0,-1), prop, m)); mesh.addTriangulo(Triangulo(v0, v3, v2, Vetor(0,0,-1), prop, m)); // Trás
    mesh.addTriangulo(Triangulo(v1, v2, v6, Vetor(1,0,0), prop, m));  mesh.addTriangulo(Triangulo(v1, v6, v5, Vetor(1,0,0), prop, m));  // Direita
    mesh.addTriangulo(Triangulo(v0, v7, v3, Vetor(-1,0,0), prop, m)); mesh.addTriangulo(Triangulo(v0, v4, v7, Vetor(-1,0,0), prop, m)); // Esquerda
    mesh.addTriangulo(Triangulo(v3, v7, v6, Vetor(0,1,0), prop, m));  mesh.addTriangulo(Triangulo(v3, v6, v2, Vetor(0,1,0), prop, m));  // Cima
    mesh.addTriangulo(Triangulo(v0, v1, v5, Vetor(0,-1,0), prop, m)); mesh.addTriangulo(Triangulo(v0, v5, v4, Vetor(0,-1,0), prop, m)); // Baixo
}

bool Paralelepipedo::intersecta(const Ray& ray, float& ti) const {
    Triangulo* tri_hit = nullptr;
    if (mesh.intersecta(ray, ti, tri_hit)) {
        ultimoTrianguloAtingido = tri_hit;
        return true;
    }
    return false;
}

Vetor Paralelepipedo::calcularNormal(const Ponto& p) const {
    if (ultimoTrianguloAtingido != nullptr) {
        return ultimoTrianguloAtingido->n;
    }
    return Vetor(0, 1, 0); 
}

Cor Paralelepipedo::getCorTextura(const Ponto& ponto) const {
    if (!temTexturaFlag || textura == nullptr) return prop.Kdif;

    Vetor v = ponto - centro;
    float u, v_coord;
    float eps = 1e-3f;

    if (fabs(fabs(v.x) - largura/2.0f) < eps) {
        u = (v.z / largura) + 0.5f;
        v_coord = (v.y / altura) + 0.5f;
    } else if (fabs(fabs(v.y) - altura/2.0f) < eps) {
        u = (v.x / largura) + 0.5f;
        v_coord = (v.z / comprimento) + 0.5f;
    } else {
        u = (v.x / largura) + 0.5f;
        v_coord = (v.y / altura) + 0.5f;
    }

    return textura->amostrar(u * escalaTextura, v_coord * escalaTextura);
}

// ============== IMPLEMENTAÇÕES DAS TRANSFORMAÇÕES ==============

void Paralelepipedo::transforma(const Matriz4x4& M) {
    centro = M * centro;
    mesh.transforma(M); 
}

void Paralelepipedo::transladar(float tx, float ty, float tz) {
    Matriz4x4 T = Transformacao::translacao(tx, ty, tz);
    transforma(T);
}

void Paralelepipedo::escalar(float sx, float sy, float sz, Ponto ponto_fixo) {
    Matriz4x4 S = Transformacao::escala(sx, sy, sz, ponto_fixo);
    centro = S * centro;
    largura *= sx;
    altura *= sy;
    comprimento *= sz;
    inicializarMesh(); 
}

void Paralelepipedo::rotacionarX(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto c = getCentro();  
    Matriz4x4 T1 = Transformacao::translacao(-c.x, -c.y, -c.z);
    Matriz4x4 R = Transformacao::rotacaoX(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(c.x, c.y, c.z);
    Matriz4x4 M = T2 * R * T1;
    transforma(M);
}

void Paralelepipedo::rotacionarY(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto c = getCentro();  
    Matriz4x4 T1 = Transformacao::translacao(-c.x, -c.y, -c.z);
    Matriz4x4 R = Transformacao::rotacaoY(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(c.x, c.y, c.z);
    Matriz4x4 M = T2 * R * T1;
    transforma(M);
}

void Paralelepipedo::rotacionarZ(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto c = getCentro();  
    Matriz4x4 T1 = Transformacao::translacao(-c.x, -c.y, -c.z);
    Matriz4x4 R = Transformacao::rotacaoZ(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(c.x, c.y, c.z);
    Matriz4x4 M = T2 * R * T1;
    transforma(M);
}

void Paralelepipedo::rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus, Ponto ponto) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 R = Transformacao::rotacaoEixoArbitrarioPonto(eixo, anguloRad, ponto);
    transforma(R);
}

void Paralelepipedo::cisalharX_XZ(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto c = getCentro();
    Matriz4x4 T1 = Transformacao::translacao(-c.x, -c.y, -c.z);
    Matriz4x4 C  = Transformacao::cisalhamentoX_XZ(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(c.x, c.y, c.z);
    transforma(T2 * C * T1);
}

void Paralelepipedo::cisalharY_XZ(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto c = getCentro();
    Matriz4x4 T1 = Transformacao::translacao(-c.x, -c.y, -c.z);
    Matriz4x4 C  = Transformacao::cisalhamentoY_XZ(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(c.x, c.y, c.z);
    transforma(T2 * C * T1);
}

void Paralelepipedo::cisalharY_XY(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto c = getCentro();
    Matriz4x4 T1 = Transformacao::translacao(-c.x, -c.y, -c.z);
    Matriz4x4 C  = Transformacao::cisalhamentoY_XY(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(c.x, c.y, c.z);
    transforma(T2 * C * T1);
}

void Paralelepipedo::cisalharZ_XY(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto c = getCentro();
    Matriz4x4 T1 = Transformacao::translacao(-c.x, -c.y, -c.z);
    Matriz4x4 C  = Transformacao::cisalhamentoZ_XY(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(c.x, c.y, c.z);
    transforma(T2 * C * T1);
}

void Paralelepipedo::espelharXY() {
    transforma(Transformacao::espelhamentoXY());
}

void Paralelepipedo::espelharXZ() {
    transforma(Transformacao::espelhamentoXZ());
}

void Paralelepipedo::espelharYZ() {
    transforma(Transformacao::espelhamentoYZ());
}