#include "objetos/Cubo.h"
#include <cmath>

int Cubo::nextId = 0;

Cubo::Cubo(Ponto centro, float lado, Propriedades prop, int m)
    : centro(centro), lado(lado), prop(prop), m(m),
      textura(nullptr), temTexturaFlag(false) {
    id = nextId++;
    inicializarMesh();
}

void Cubo::inicializarMesh() {
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
    
    // Frente (z+)
    mesh.addTriangulo(Triangulo(v4, v5, v6, n_frente, prop, m));
    mesh.addTriangulo(Triangulo(v4, v6, v7, n_frente, prop, m));
    
    // Trás (z-)
    mesh.addTriangulo(Triangulo(v0, v2, v1, n_tras, prop, m));
    mesh.addTriangulo(Triangulo(v0, v3, v2, n_tras, prop, m));
    
    // Direita (x+)
    mesh.addTriangulo(Triangulo(v1, v2, v6, n_direita, prop, m));
    mesh.addTriangulo(Triangulo(v1, v6, v5, n_direita, prop, m));
    
    // Esquerda (x-)
    mesh.addTriangulo(Triangulo(v0, v7, v3, n_esquerda, prop, m));
    mesh.addTriangulo(Triangulo(v0, v4, v7, n_esquerda, prop, m));
    
    // Cima (y+)
    mesh.addTriangulo(Triangulo(v3, v7, v6, n_cima, prop, m));
    mesh.addTriangulo(Triangulo(v3, v6, v2, n_cima, prop, m));
    
    // Baixo (y-)
    mesh.addTriangulo(Triangulo(v0, v1, v5, n_baixo, prop, m));
    mesh.addTriangulo(Triangulo(v0, v5, v4, n_baixo, prop, m));
}

bool Cubo::intersecta(const Ray& ray, float& ti) const {
    Triangulo* tri_hit = nullptr;
    return mesh.intersecta(ray, ti, tri_hit);
}

Vetor Cubo::calcularNormal(const Ponto& p) const {
    Vetor v = p - centro;
    
    float absX = fabs(v.x);
    float absY = fabs(v.y);
    float absZ = fabs(v.z);
    
    if (absX >= absY && absX >= absZ) {
        return Vetor((v.x > 0) ? 1.0f : -1.0f, 0.0f, 0.0f);
    } else if (absY >= absX && absY >= absZ) {
        return Vetor(0.0f, (v.y > 0) ? 1.0f : -1.0f, 0.0f);
    } else {
        return Vetor(0.0f, 0.0f, (v.z > 0) ? 1.0f : -1.0f);
    }
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
        
        return textura->amostrar(u, vCoord);
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