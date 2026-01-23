#include "objetos/Cone.h"
#include "objetos/Plano.h"
#include <cmath>
#include <algorithm>
#include <vector>


Cone::Cone(Ponto CB, float rbCone, float altura, Vetor dco, Propriedades prop, int m)
    : CB(CB), rbCone(rbCone), altura(altura), dco(normalizar(dco)), 
      prop(prop), m(m), textura(nullptr), temTexturaFlag(false) {
    id = 0;
}

bool Cone::intersecta(const Ray& ray, float& t) const {
    Ponto vertice = getVertice();
    float razao = calcularRazao();
    
    Vetor w = ray.P_0 - vertice;
    
    float a = produto_escalar(ray.dr, ray.dr) - (1 + razao*razao) * pow(produto_escalar(ray.dr, dco), 2);
    float b = 2 * (produto_escalar(ray.dr, w) - (1 + razao*razao) * produto_escalar(ray.dr, dco) * produto_escalar(w, dco));
    float c = produto_escalar(w, w) - (1 + razao*razao) * pow(produto_escalar(w, dco), 2);
    
    float delta = b*b - 4*a*c;
    if (delta < 0) return false;
    
    float t1 = (-b - sqrt(delta)) / (2*a);
    float t2 = (-b + sqrt(delta)) / (2*a);
    
    std::vector<float> ts_validos;
    
    if (t1 > 0) {
        Ponto P1 = ray.P_0 + (ray.dr * t1);
        float h1 = produto_escalar(P1 - CB, dco);
        if (h1 >= 0 && h1 <= altura) ts_validos.push_back(t1);
    }
    
    if (t2 > 0) {
        Ponto P2 = ray.P_0 + (ray.dr * t2);
        float h2 = produto_escalar(P2 - CB, dco);
        if (h2 >= 0 && h2 <= altura) ts_validos.push_back(t2);
    }
    
    float t_base;
    Plano plano_base(CB, dco, prop, m);
    if (plano_base.intersecta(ray, t_base)) {
        Ponto P_base = ray.P_0 + (ray.dr * t_base);
        if (comprimento(P_base - CB) <= rbCone) {
            ts_validos.push_back(t_base);
        }
    }
    
    if (ts_validos.empty()) return false;
    
    t = *std::min_element(ts_validos.begin(), ts_validos.end());
    return true;
}

Vetor Cone::calcularNormal(const Ponto& P) const {
    Ponto vertice = getVertice();
    
    float h_base = produto_escalar(P - CB, -dco);
    if (fabs(h_base) < 1e-4 && comprimento(P - CB) <= rbCone) {
        return -dco;
    }
    
    Vetor lateral = P - vertice;
    Vetor proj(lateral.x - dco.x * produto_escalar(lateral, dco),
               lateral.y - dco.y * produto_escalar(lateral, dco),
               lateral.z - dco.z * produto_escalar(lateral, dco));
    
    return normalizar(proj);
}

Propriedades Cone::getPropriedades() const {
    return prop;
}

int Cone::getMaterial() const {
    return m;
}

std::string Cone::getNome() const {
    return "Cone";
}

int Cone::getId() const {
    return id;
}

Cor Cone::getCorTextura(const Ponto& ponto) const {
    if (temTexturaFlag && textura != nullptr) {
        Ponto vertice = getVertice();
        Vetor vetorPonto = ponto - vertice;
        
        Vetor vetorBase = ponto - CB;
        if (fabs(produto_escalar(vetorBase, dco)) < 1e-4f) {
            float u = (vetorBase.x / (2 * rbCone)) + 0.5f;
            float v = (vetorBase.z / (2 * rbCone)) + 0.5f;
            return textura->amostrar(u, v);
        }
        
        float h = produto_escalar(vetorPonto, dco);
        Vetor projecao = dco * h;
        Ponto pontoNoEixo = vertice + projecao;
        Vetor radial = ponto - pontoNoEixo;
        
        if (comprimento(radial) > 1e-4f) {
            radial = normalizar(radial);
        }
        
        Vetor referencia;
        if (fabs(dco.x) > fabs(dco.y)) {
            referencia = Vetor(dco.z, 0, -dco.x);
        } else {
            referencia = Vetor(0, -dco.z, dco.y);
        }
        referencia = normalizar(referencia);
        
        float angulo = std::acos(produto_escalar(radial, referencia));
        if (produto_escalar(produto_vetorial(referencia, radial), dco) < 0) {
            angulo = 2 * M_PI - angulo;
        }
        
        float u = angulo / (2 * M_PI);
        float v = (altura + h) / altura; 
        
        return textura->amostrar(u, v);
    }
    return prop.Kdif;
}

bool Cone::temTextura() const {
    return temTexturaFlag;
}

void Cone::setTextura(Textura* tex) {
    textura = tex;
    temTexturaFlag = (tex != nullptr);
}

Ponto Cone::getVertice() const {
    return CB + (dco * altura);
}

float Cone::calcularRazao() const {
    return rbCone / altura;
}

// ========== IMPLEMENTAÇÕES DAS TRANSFORMAÇÕES ==========

void Cone::transforma(const Matriz4x4& M) {
    CB = M * CB;
    dco = normalizar(M * dco);
}