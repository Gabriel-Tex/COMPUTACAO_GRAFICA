#include "objetos/Cilindro.h"
using namespace std;

// ============== CLASSE ==============
Cilindro::Cilindro(Ponto CB, float rbCil, float altura, Vetor dcil, Propriedades prop, int m){
    this->CB = CB;
    this->rbCil = rbCil;
    this->altura = altura;
    this->dcil = dcil;
    this->prop = prop;
    this->m = m;
}

// ============== MÉTODOS ASSOCIADOS A CILINDRO ==============

bool IntersecaoRayCilindro(Cilindro cilindro, Ray ray, float &t) {
    // Vetor do centro da base para a origem do raio
    Vetor w = ray.P_0 - cilindro.CB;
    
    // Projeções nos eixos do cilindro
    float a = produto_escalar(ray.dr, ray.dr) - pow(produto_escalar(ray.dr, cilindro.dcil), 2);
    float b = 2 * (produto_escalar(ray.dr, w) - produto_escalar(ray.dr, cilindro.dcil) * produto_escalar(w, cilindro.dcil));
    float c = produto_escalar(w, w) - pow(produto_escalar(w, cilindro.dcil), 2) - cilindro.rbCil * cilindro.rbCil;
    
    float delta = b*b - 4*a*c;
    if (delta < 0) return false;
    
    float t1 = (-b - sqrt(delta)) / (2*a);
    float t2 = (-b + sqrt(delta)) / (2*a);
    
    // Verificar se as interseções estão dentro da altura do cilindro
    vector<float> ts_validos;
    
    if (t1 > 0) {
        Ponto P1 (  ray.P_0.x + ray.dr.x * t1,
                    ray.P_0.y + ray.dr.y * t1,
                    ray.P_0.z + ray.dr.z * t1  );
        float h1 = produto_escalar(P1 - cilindro.CB, cilindro.dcil);
        if (h1 >= 0 && h1 <= cilindro.altura) ts_validos.push_back(t1);
    }
    
    if (t2 > 0) {
        Ponto P2 (  ray.P_0.x + ray.dr.x * t2,
                    ray.P_0.y + ray.dr.y * t2,
                    ray.P_0.z + ray.dr.z * t2  );

        float h2 = produto_escalar(P2 - cilindro.CB, cilindro.dcil);
        if (h2 >= 0 && h2 <= cilindro.altura) ts_validos.push_back(t2);
    }
    

    // Verificar tampas
    // tampa inferior
    Plano base(cilindro.CB, -cilindro.dcil, cilindro.prop, cilindro.m);
    float t_base;
    if (IntersecaoRayPlano(base, ray, t_base)) {
        Ponto P_base (  ray.P_0.x + ray.dr.x * t_base,
                        ray.P_0.y + ray.dr.y * t_base,
                        ray.P_0.z + ray.dr.z * t_base  );
        if (comprimento(P_base - cilindro.CB) <= cilindro.rbCil) {
            ts_validos.push_back(t_base);
        }
    }
    
    // tampa superior
    Ponto centro_topo ( cilindro.altura * cilindro.dcil.x + cilindro.CB.x,
                        cilindro.altura * cilindro.dcil.y + cilindro.CB.y,
                        cilindro.altura * cilindro.dcil.z + cilindro.CB.z );
    
    Plano topo(centro_topo, cilindro.dcil, cilindro.prop, cilindro.m);
    float t_topo;
    if (IntersecaoRayPlano(topo, ray, t_topo)) {
        Ponto P_topo (  ray.P_0.x + ray.dr.x * t_topo,
                        ray.P_0.y + ray.dr.y * t_topo,
                        ray.P_0.z + ray.dr.z * t_topo  );
        if (comprimento(P_topo - centro_topo) <= cilindro.rbCil) {
            ts_validos.push_back(t_topo);
        }
    }
    
    if (ts_validos.empty()) return false;
    
    t = *min_element(ts_validos.begin(), ts_validos.end());
    return true;
}

// NORMAL DO CILINDRO
Vetor normalCilindro(Cilindro cilindro, Ponto P) {
    // Calcular a altura ao longo do eixo do cilindro
    float h = produto_escalar(P - cilindro.CB, cilindro.dcil);
    
    // Verificar se está na tampa inferior (usando tolerância mais inteligente)
    if (h >= -1e-4 && h <= 1e-4) {
        return -cilindro.dcil;
    }
    
    // Verificar se está na tampa superior
    Ponto centro_topo(
        cilindro.CB.x + cilindro.dcil.x * cilindro.altura,
        cilindro.CB.y + cilindro.dcil.y * cilindro.altura,
        cilindro.CB.z + cilindro.dcil.z * cilindro.altura
    );
    
    float h_topo = produto_escalar(P - centro_topo, cilindro.dcil);
    if (h_topo >= -1e-4 && h_topo <= 1e-4) {
        return cilindro.dcil;
    }
    
    // Se chegou aqui, está na superfície lateral
    // Calcular o Ponto correspondente no eixo do cilindro
    Ponto ponto_no_eixo(
        cilindro.CB.x + cilindro.dcil.x * h,
        cilindro.CB.y + cilindro.dcil.y * h,
        cilindro.CB.z + cilindro.dcil.z * h
    );
    
    // A normal é o Vetor do Ponto no eixo até o Ponto na superfície
    return normalizar(P - ponto_no_eixo);
}