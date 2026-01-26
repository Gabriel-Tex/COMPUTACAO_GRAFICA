#include "objetos/Cubo.h"
#include <cmath>

Cubo::Cubo(Ponto centro, float lado, Propriedades prop, int m)
    : centro(centro), lado(lado), prop(prop), m(m),
      textura(nullptr), temTexturaFlag(false), ultimoTrianguloAtingido(nullptr), escalaTextura(1.0f) {
    id = 0;
    inicializarMesh();
}

void Cubo::inicializarMesh() {
    mesh.limpar(); 
    float h = lado / 2.0f;
    
    Ponto v0(centro.x - h, centro.y - h, centro.z - h);
    Ponto v1(centro.x + h, centro.y - h, centro.z - h);
    Ponto v2(centro.x + h, centro.y + h, centro.z - h);
    Ponto v3(centro.x - h, centro.y + h, centro.z - h);
    Ponto v4(centro.x - h, centro.y - h, centro.z + h);
    Ponto v5(centro.x + h, centro.y - h, centro.z + h);
    Ponto v6(centro.x + h, centro.y + h, centro.z + h);
    Ponto v7(centro.x - h, centro.y + h, centro.z + h);
    
    Vetor n_frente (0, 0, 1);
    Vetor n_tras   (0, 0, -1);
    Vetor n_direita(1, 0, 0);
    Vetor n_esquerda(-1, 0, 0);
    Vetor n_cima   (0, 1, 0);
    Vetor n_baixo  (0, -1, 0);
    
    mesh.addTriangulo(Triangulo(v4, v5, v6, n_frente, prop, m));
    mesh.addTriangulo(Triangulo(v4, v6, v7, n_frente, prop, m));
    mesh.addTriangulo(Triangulo(v0, v2, v1, n_tras, prop, m));
    mesh.addTriangulo(Triangulo(v0, v3, v2, n_tras, prop, m));
    mesh.addTriangulo(Triangulo(v1, v2, v6, n_direita, prop, m));
    mesh.addTriangulo(Triangulo(v1, v6, v5, n_direita, prop, m));
    mesh.addTriangulo(Triangulo(v0, v7, v3, n_esquerda, prop, m));
    mesh.addTriangulo(Triangulo(v0, v4, v7, n_esquerda, prop, m));
    mesh.addTriangulo(Triangulo(v3, v7, v6, n_cima, prop, m));
    mesh.addTriangulo(Triangulo(v3, v6, v2, n_cima, prop, m));
    mesh.addTriangulo(Triangulo(v0, v1, v5, n_baixo, prop, m));
    mesh.addTriangulo(Triangulo(v0, v5, v4, n_baixo, prop, m));
}

bool Cubo::intersecta(const Ray& ray, float& ti) const {
    Triangulo* tri_hit = nullptr;
    if (mesh.intersecta(ray, ti, tri_hit)) {
        ultimoTrianguloAtingido = tri_hit;
        return true;
    }
    return false;
}

Vetor Cubo::calcularNormal(const Ponto& p) const {
    if (ultimoTrianguloAtingido != nullptr) {
        return ultimoTrianguloAtingido->n;
    }
    return Vetor(0, 1, 0);
}


Propriedades Cubo::getPropriedades() const {
    return prop;
}

int Cubo::getMaterial() const {
    return m;
}

std::string Cubo::getNome() const {
    return "Cubo";
}

int Cubo::getId() const {
    return id;
}

Cor Cubo::getCorTextura(const Ponto& ponto) const {
    if (temTexturaFlag && textura != nullptr) {
        Vetor v = ponto - centro;
        float h = lado / 2.0f;
        
        float absX = fabs(v.x);
        float absY = fabs(v.y);
        float absZ = fabs(v.z);
        
        float u, vCoord;
        
        if (absX >= absY && absX >= absZ) {
            // Face X
            u = (v.z / lado) + 0.5f;
            vCoord = (v.y / lado) + 0.5f;
        } else if (absY >= absX && absY >= absZ) {
            // Face Y
            u = (v.x / lado) + 0.5f;
            vCoord = (v.z / lado) + 0.5f;
        } else {
            // Face Z
            u = (v.x / lado) + 0.5f;
            vCoord = (v.y / lado) + 0.5f;
        }
        
        return textura->amostrar(u * escalaTextura, vCoord * escalaTextura);
    }
    return prop.Kdif;
}

bool Cubo::temTextura() const {
    return temTexturaFlag;
}

void Cubo::setTextura(Textura* tex) {
    textura = tex;
    temTexturaFlag = (tex != nullptr);
}

Vetor Cubo::normalNoPonto(Ponto p) const {
    return calcularNormal(p);
}

// ========== IMPLEMENTAÇÕES DAS TRANSFORMAÇÕES ==========

void Cubo::transforma(const Matriz4x4& M) {
    centro = M * centro; 
    mesh.transforma(M);  
}

void Cubo::transladar(float tx, float ty, float tz) {
    Matriz4x4 T = Transformacao::translacao(tx, ty, tz);
    transforma(T);
}

void Cubo::escalar(float s, Ponto ponto_fixo) {
    Matriz4x4 S = Transformacao::escala(s, s, s, ponto_fixo);
    centro = S * centro;
    lado *= s;
    inicializarMesh(); 
}

void Cubo::rotacionarX(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto centro = getCentro();  
    Matriz4x4 T1 = Transformacao::translacao(-centro.x, -centro.y, -centro.z);
    Matriz4x4 R = Transformacao::rotacaoX(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(centro.x, centro.y, centro.z);
    Matriz4x4 M = T2 * R * T1;
    transforma(M);
}

void Cubo::rotacionarY(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto centro = getCentro();  
    Matriz4x4 T1 = Transformacao::translacao(-centro.x, -centro.y, -centro.z);
    Matriz4x4 R = Transformacao::rotacaoY(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(centro.x, centro.y, centro.z);
    Matriz4x4 M = T2 * R * T1;
    transforma(M);
}

void Cubo::rotacionarZ(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Ponto centro = getCentro();  
    Matriz4x4 T1 = Transformacao::translacao(-centro.x, -centro.y, -centro.z);
    Matriz4x4 R = Transformacao::rotacaoZ(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(centro.x, centro.y, centro.z);
    Matriz4x4 M = T2 * R * T1;
    transforma(M);
}

void Cubo::cisalharX_XZ(float angulo) {
    Ponto base = getCentro();
    Matriz4x4 T1 = Transformacao::translacao(-base.x, -base.y, -base.z);
    Matriz4x4 C = Transformacao::cisalhamentoX_XZ(angulo);
    Matriz4x4 T2 = Transformacao::translacao(base.x, base.y, base.z);
    Matriz4x4 M = T2 * C * T1;
    transforma(M);
}

void Cubo::cisalharY_XY(float angulo) {
    Ponto base = getCentro();
    Matriz4x4 T1 = Transformacao::translacao(-base.x, -base.y, -base.z);
    Matriz4x4 C = Transformacao::cisalhamentoY_XY(angulo);
    Matriz4x4 T2 = Transformacao::translacao(base.x, base.y, base.z);
    Matriz4x4 M = T2 * C * T1;
    transforma(M);
}

void Cubo::cisalharY_XZ(float angulo) {
    Ponto base = getCentro();
    Matriz4x4 T1 = Transformacao::translacao(-base.x, -base.y, -base.z);
    Matriz4x4 C = Transformacao::cisalhamentoY_XZ(angulo);
    Matriz4x4 T2 = Transformacao::translacao(base.x, base.y, base.z);
    Matriz4x4 M = T2 * C * T1;
    transforma(M);
}

void Cubo::cisalharZ_XY(float angulo) {
    Ponto base = getCentro();
    Matriz4x4 T1 = Transformacao::translacao(-base.x, -base.y, -base.z);
    Matriz4x4 C = Transformacao::cisalhamentoZ_XY(angulo);
    Matriz4x4 T2 = Transformacao::translacao(base.x, base.y, base.z);
    Matriz4x4 M = T2 * C * T1;
    transforma(M);
}

void Cubo::espelharXY() {
    Matriz4x4 E = Transformacao::espelhamentoXY();
    transforma(E);
}

void Cubo::espelharXZ() {
    Matriz4x4 E = Transformacao::espelhamentoXZ();
    transforma(E);
}

void Cubo::espelharYZ() {
    Matriz4x4 E = Transformacao::espelhamentoYZ();
    transforma(E);
}

void Cubo::rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus, Ponto ponto) {
    Matriz4x4 R = Transformacao::rotacaoEixoArbitrarioPonto(eixo, anguloGraus, ponto);
    transforma(R);
}


