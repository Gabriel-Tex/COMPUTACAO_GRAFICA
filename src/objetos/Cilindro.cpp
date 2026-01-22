#include "objetos/Cilindro.h"
#include "objetos/Plano.h" 
#include <cmath>
#include <algorithm>
#include <vector>

int Cilindro::nextId = 0;

Cilindro::Cilindro(Ponto CB, float rbCil, float altura, Vetor dcil, Propriedades prop, int m)
    : CB(CB), rbCil(rbCil), altura(altura), dcil(normalizar(dcil)), 
      prop(prop), m(m), textura(nullptr), temTexturaFlag(false) {
    id = nextId++;
}

bool Cilindro::intersecta(const Ray& ray, float& t) const {
    Vetor w = ray.P_0 - CB;
    
    float a = produto_escalar(ray.dr, ray.dr) - pow(produto_escalar(ray.dr, dcil), 2);
    float b = 2 * (produto_escalar(ray.dr, w) - produto_escalar(ray.dr, dcil) * produto_escalar(w, dcil));
    float c = produto_escalar(w, w) - pow(produto_escalar(w, dcil), 2) - rbCil * rbCil;
    
    float delta = b*b - 4*a*c;
    if (delta < 0) return false;
    
    float t1 = (-b - sqrt(delta)) / (2*a);
    float t2 = (-b + sqrt(delta)) / (2*a);
    
    std::vector<float> ts_validos;
    
    if (t1 > 0) {
        Ponto P1 = ray.P_0 + (ray.dr * t1);
        float h1 = produto_escalar(P1 - CB, dcil);
        if (h1 >= 0 && h1 <= altura) ts_validos.push_back(t1);
    }
    
    if (t2 > 0) {
        Ponto P2 = ray.P_0 + (ray.dr * t2);
        float h2 = produto_escalar(P2 - CB, dcil);
        if (h2 >= 0 && h2 <= altura) ts_validos.push_back(t2);
    }
    
    Plano base(CB, -dcil, prop, m);
    float t_base;
    if (base.intersecta(ray, t_base)) {
        Ponto P_base = ray.P_0 + (ray.dr * t_base);
        if (comprimento(P_base - CB) <= rbCil) {
            ts_validos.push_back(t_base);
        }
    }
    
    Ponto centro_topo = calcularTopo();
    Plano topo(centro_topo, dcil, prop, m);
    float t_topo;
    if (topo.intersecta(ray, t_topo)) {
        Ponto P_topo = ray.P_0 + (ray.dr * t_topo);
        if (comprimento(P_topo - centro_topo) <= rbCil) {
            ts_validos.push_back(t_topo);
        }
    }
    
    if (ts_validos.empty()) return false;
    
    t = *std::min_element(ts_validos.begin(), ts_validos.end());
    return true;
}

Vetor Cilindro::calcularNormal(const Ponto& P) const {
    float h = produto_escalar(P - CB, dcil);
    
    if (h >= -1e-4 && h <= 1e-4) {
        return -dcil;
    }
    
    Ponto centro_topo = calcularTopo();
    float h_topo = produto_escalar(P - centro_topo, dcil);
    if (h_topo >= -1e-4 && h_topo <= 1e-4) {
        return dcil;
    }
    
    Ponto ponto_no_eixo = CB + (dcil * h);
    return normalizar(P - ponto_no_eixo);
}

Propriedades Cilindro::getPropriedades() const {
    return prop;
}

int Cilindro::getMaterial() const {
    return m;
}

std::string Cilindro::getNome() const {
    return "Cilindro";
}

int Cilindro::getId() const {
    return id;
}

Cor Cilindro::getCorTextura(const Ponto& ponto) const {
    if (temTexturaFlag && textura != nullptr) {
        Vetor vetorPonto = ponto - CB;
        float h = produto_escalar(vetorPonto, dcil);
        Vetor projecao = dcil * h;
        Ponto pontoNoEixo = CB + projecao;
        Vetor radial = ponto - pontoNoEixo;
        
        if (comprimento(radial) > 1e-4f) {
            radial = normalizar(radial);
        }
        
        Vetor referencia;
        if (fabs(dcil.x) > fabs(dcil.y)) {
            referencia = Vetor(dcil.z, 0, -dcil.x);
        } else {
            referencia = Vetor(0, -dcil.z, dcil.y);
        }
        referencia = normalizar(referencia);
        
        float angulo = std::acos(produto_escalar(radial, referencia));
        if (produto_escalar(produto_vetorial(referencia, radial), dcil) < 0) {
            angulo = 2 * M_PI - angulo;
        }
        
        float u = angulo / (2 * M_PI);
        float v = h / altura;
        
        return textura->amostrar(u, v);
    }
    return prop.Kdif;
}

bool Cilindro::temTextura() const {
    return temTexturaFlag;
}

void Cilindro::setTextura(Textura* tex) {
    textura = tex;
    temTexturaFlag = (tex != nullptr);
}

bool Cilindro::verificarAltura(const Ponto& ponto) const {
    float h = produto_escalar(ponto - CB, dcil);
    return h >= 0 && h <= altura;
}

Ponto Cilindro::calcularTopo() const {
    return CB + (dcil * altura);
}