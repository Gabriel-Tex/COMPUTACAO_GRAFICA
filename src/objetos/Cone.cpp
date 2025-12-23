#include "objetos/Cone.h"
using namespace std;

// ============== CLASSE ==============
Cone::Cone(Ponto CB, float rbCone, float altura, Vetor dco, Propriedades prop, int m){
    this->CB = CB;
    this->rbCone = rbCone;
    this->altura = altura;
    this->dco = dco;
    this->prop = prop;
    this->m = m;
}

// ============== MÉTODOS ASSOCIADOS A CONE ==============
                                    // a origem tem de ser corrigida
bool IntersecaoRayCone(Cone cone, Ray ray, float &t) {
    Ponto vertice ( cone.CB.x + cone.dco.x * cone.altura,
                    cone.CB.y + cone.dco.y * cone.altura,
                    cone.CB.z + cone.dco.z * cone.altura); // Vértice do cone
    float razao = cone.rbCone / cone.altura; // tan(θ)
    
    Vetor w = ray.P_0 - vertice;
    
    float a = produto_escalar(ray.dr, ray.dr) - (1 + razao*razao) * pow(produto_escalar(ray.dr, cone.dco), 2);
    float b = 2 * (produto_escalar(ray.dr, w) - (1 + razao*razao) * produto_escalar(ray.dr, cone.dco) * produto_escalar(w, cone.dco));
    float c = produto_escalar(w, w) - (1 + razao*razao) * pow(produto_escalar(w, cone.dco), 2);
    
    float delta = b*b - 4*a*c;
    if (delta < 0) return false;
    
    float t1 = (-b - sqrt(delta)) / (2*a);
    float t2 = (-b + sqrt(delta)) / (2*a);
    
    vector<float> ts_validos;
    
    // Verificar interseções com a superfície lateral
    if (t1 > 0) {
        Ponto P1 (  ray.P_0.x + ray.dr.x * t1,
                    ray.P_0.y + ray.dr.y * t1,
                    ray.P_0.z + ray.dr.z * t1  );
        float h1 = produto_escalar(P1 - cone.CB, cone.dco);
        if (h1 >= 0 && h1 <= cone.altura) ts_validos.push_back(t1);
    }
    
    if (t2 > 0) {
        Ponto P2 (  ray.P_0.x + ray.dr.x * t2,
                    ray.P_0.y + ray.dr.y * t2,
                    ray.P_0.z + ray.dr.z * t2  );
        float h2 = produto_escalar(P2 - cone.CB, cone.dco);
        if (h2 >= 0 && h2 <= cone.altura) ts_validos.push_back(t2);
    }
    
    // Verificar base do cone
    float t_base;
    Plano plano_base(cone.CB, cone.dco, cone.prop, cone.m);
    if (IntersecaoRayPlano(plano_base, ray, t_base)) {
        Ponto P_base (  ray.P_0.x + ray.dr.x * t_base,
                        ray.P_0.y + ray.dr.y * t_base,
                        ray.P_0.z + ray.dr.z * t_base  );
        if (comprimento(P_base - cone.CB)<= cone.rbCone) {
            ts_validos.push_back(t_base);
        }
    }
    
    if (ts_validos.empty()) return false;
    
    t = *min_element(ts_validos.begin(), ts_validos.end());
    return true;
}

// NORMAL DO CONE NO PONTO P
Vetor normalCone(Cone cone, Ponto P) {
    Ponto vertice ( cone.CB.x + cone.dco.x * cone.altura,
                    cone.CB.y + cone.dco.y * cone.altura,
                    cone.CB.z + cone.dco.z * cone.altura );
    
    // Verificar base
    float h_base = produto_escalar(P - cone.CB, -cone.dco);
    if (fabs(h_base) < 1e-4 && comprimento(P - cone.CB) <= cone.rbCone) {
        return -cone.dco; // Normal da base
    }
    
    // Superfície lateral
    Ponto projecao (cone.CB.x + cone.dco.x * produto_escalar(P - cone.CB, cone.dco),
                    cone.CB.y + cone.dco.y * produto_escalar(P - cone.CB, cone.dco),
                    cone.CB.z + cone.dco.z * produto_escalar(P - cone.CB, cone.dco));
    Vetor lateral = P - vertice;
    //Vetor eixo = cone.CB - vertice;
    
    // Projeção no plano perpendicular ao eixo
    Vetor proj (lateral.x - cone.dco.x * produto_escalar(lateral, cone.dco),
                lateral.y - cone.dco.y * produto_escalar(lateral, cone.dco),
                lateral.z - cone.dco.z * produto_escalar(lateral, cone.dco));

    return normalizar(proj);
}
