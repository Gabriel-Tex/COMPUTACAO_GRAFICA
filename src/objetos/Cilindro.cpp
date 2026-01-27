#include "objetos/Cilindro.h"
#include "objetos/Plano.h" 
#include <cmath>
#include <algorithm>
#include <vector>

Cilindro::Cilindro(Ponto CB, float rbCil, float altura, Vetor dcil, Propriedades prop, int m, bool temBase, bool temTopo)
    : CB(CB), rbCil(rbCil), altura(altura), dcil(normalizar(dcil)), 
      prop(prop), m(m), textura(nullptr), temTexturaFlag(false), 
      temBase(temBase), temTopo(temTopo) {
    id = 0;
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
    
    if (temBase) {
        Plano base(CB, -dcil, prop, m);
        float t_base;
        if (base.intersecta(ray, t_base)) {
            Ponto P_base = ray.P_0 + (ray.dr * t_base);
            if (comprimento(P_base - CB) <= rbCil) {
                ts_validos.push_back(t_base);
            }
        }
    }
    
    if (temTopo) {
        Ponto centro_topo = calcularTopo();
        Plano topo(centro_topo, dcil, prop, m);
        float t_topo;
        if (topo.intersecta(ray, t_topo)) {
            Ponto P_topo = ray.P_0 + (ray.dr * t_topo);
            if (comprimento(P_topo - centro_topo) <= rbCil) {
                ts_validos.push_back(t_topo);
            }
        }
    }
    
    if (ts_validos.empty()) return false;
    
    t = *std::min_element(ts_validos.begin(), ts_validos.end());
    return true;
}

Vetor Cilindro::calcularNormal(const Ponto& P) const {
    const float EPS = 1e-3f;
    float h = produto_escalar(P - CB, dcil);

    if (temBase && fabs(h) < EPS) {
        return -dcil; 
    }

    if (temTopo && fabs(h - altura) < EPS) {
        return -dcil; 
    }

    Ponto pontoNoEixo = CB + (dcil * h);
    Vetor normalLateral = normalizar(P - pontoNoEixo);
    
    return normalLateral;
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
    if (!temTexturaFlag || textura == nullptr) return prop.Kdif;

    Vetor vetorPonto = ponto - CB;
    float h = produto_escalar(vetorPonto, dcil);
    
    float v_coord = std::max(0.0f, std::min(1.0f, h / altura));

    if (fabs(h) < 1e-3f || fabs(h - altura) < 1e-3f) {
        Vetor v_up = dcil;
        Vetor v_right;
        if (fabs(v_up.x) > 0.9f) v_right = normalizar(produto_vetorial(v_up, Vetor(0, 1, 0)));
        else v_right = normalizar(produto_vetorial(v_up, Vetor(1, 0, 0)));
        Vetor v_forward = normalizar(produto_vetorial(v_up, v_right));

        Vetor radialPuro = vetorPonto - (dcil * h);
        float x_local = produto_escalar(radialPuro, v_right);
        float z_local = produto_escalar(radialPuro, v_forward);

        float u = (x_local / (2.0f * rbCil)) + 0.5f;
        float v = (z_local / (2.0f * rbCil)) + 0.5f;

        return textura->amostrar(u, v);
    }

    Vetor radial = normalizar(vetorPonto - (dcil * h));
    
    Vetor v_right;
    if (fabs(dcil.x) > 0.9f) v_right = normalizar(produto_vetorial(dcil, Vetor(0, 1, 0)));
    else v_right = normalizar(produto_vetorial(dcil, Vetor(1, 0, 0)));
    
    float cos_theta = std::max(-1.0f, std::min(1.0f, produto_escalar(radial, v_right)));
    float angulo = std::acos(cos_theta);
    
    Vetor cross = produto_vetorial(v_right, radial);
    if (produto_escalar(dcil, cross) < 0) {
        angulo = 2.0f * M_PI - angulo;
    }

    float u_coord = angulo / (2.0f * M_PI);
    
    return textura->amostrar(u_coord, v_coord);
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

// ========== IMPLEMENTAÇÕES DAS TRANSFORMAÇÕES ==========

void Cilindro::transforma(const Matriz4x4& M) {
    CB = M * CB;

    Vetor dcil_com_altura = dcil * altura;
    Vetor dcil_transformado = M * dcil_com_altura; 
    
    float nova_altura = comprimento(dcil_transformado);
    Vetor novo_dcil = normalizar(dcil_transformado);

    Vetor vetor_raio;
    if (fabs(dcil.x) > fabs(dcil.y)) {
        vetor_raio = Vetor(dcil.z, 0, -dcil.x);
    } else {
        vetor_raio = Vetor(0, -dcil.z, dcil.y);
    }
    vetor_raio = normalizar(vetor_raio);
    
    Vetor raio_transformado = M * (vetor_raio * rbCil);
    float novo_raio = comprimento(raio_transformado);

    this->altura = nova_altura;
    this->dcil = novo_dcil;
    this->rbCil = novo_raio;
}

void Cilindro::transladar(float tx, float ty, float tz) {
    Matriz4x4 T = Transformacao::translacao(tx, ty, tz);
    transforma(T);
}

void Cilindro::escalar(float sx, float sy, float sz, Ponto ponto_fixo) {
    Matriz4x4 S = Transformacao::escala(sx, sy, sz, ponto_fixo);
    this->transforma(S);
}

void Cilindro::rotacionarX(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    
    Ponto base = getCentroBase(); 
    
    Matriz4x4 T1 = Transformacao::translacao(-base.x, -base.y, -base.z);
    Matriz4x4 R = Transformacao::rotacaoX(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(base.x, base.y, base.z);
  
    Matriz4x4 M = T2 * R * T1;
    
    transforma(M);
}

void Cilindro::rotacionarY(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    
    Ponto base = getCentroBase();  
    
    Matriz4x4 T1 = Transformacao::translacao(-base.x, -base.y, -base.z);
    Matriz4x4 R = Transformacao::rotacaoY(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(base.x, base.y, base.z);
  
    Matriz4x4 M = T2 * R * T1;
    
    transforma(M);
}

void Cilindro::rotacionarZ(float anguloGraus) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    
    Ponto base = getCentroBase();  
    
    Matriz4x4 T1 = Transformacao::translacao(-base.x, -base.y, -base.z);
    Matriz4x4 R = Transformacao::rotacaoZ(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(base.x, base.y, base.z);
  
    Matriz4x4 M = T2 * R * T1;
    
    transforma(M);
}

void Cilindro::cisalharX_XZ(float anguloGraus) {
    Ponto base = getCentroBase();
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 T1 = Transformacao::translacao(-base.x, -base.y, -base.z);
    Matriz4x4 C = Transformacao::cisalhamentoX_XZ(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(base.x, base.y, base.z);
    Matriz4x4 M = T2 * C * T1;
    transforma(M);
}

void Cilindro::cisalharY_XY(float anguloGraus) {
    Ponto base = getCentroBase();
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 T1 = Transformacao::translacao(-base.x, -base.y, -base.z);
    Matriz4x4 C = Transformacao::cisalhamentoY_XY(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(base.x, base.y, base.z);
    Matriz4x4 M = T2 * C * T1;
    transforma(M);
}

void Cilindro::cisalharY_XZ(float anguloGraus) {
    Ponto base = getCentroBase();
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 T1 = Transformacao::translacao(-base.x, -base.y, -base.z);
    Matriz4x4 C = Transformacao::cisalhamentoY_XZ(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(base.x, base.y, base.z);
    Matriz4x4 M = T2 * C * T1;
    transforma(M);
}

void Cilindro::cisalharZ_XY(float anguloGraus) {
    Ponto base = getCentroBase();
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 T1 = Transformacao::translacao(-base.x, -base.y, -base.z);
    Matriz4x4 C = Transformacao::cisalhamentoZ_XY(anguloRad);
    Matriz4x4 T2 = Transformacao::translacao(base.x, base.y, base.z);
    Matriz4x4 M = T2 * C * T1;
    transforma(M);
}

void Cilindro::espelharXY() {
    Matriz4x4 E = Transformacao::espelhamentoXY();
    transforma(E);
}

void Cilindro::espelharXZ() {
    Matriz4x4 E = Transformacao::espelhamentoXZ();
    transforma(E);
}

void Cilindro::espelharYZ() {
    Matriz4x4 E = Transformacao::espelhamentoYZ();
    transforma(E);
}

void Cilindro::rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus, Ponto ponto) {
    float anguloRad = Transformacao::grausParaRadianos(anguloGraus);
    Matriz4x4 R = Transformacao::rotacaoEixoArbitrarioPonto(eixo, anguloRad, ponto);
    transforma(R);
}

