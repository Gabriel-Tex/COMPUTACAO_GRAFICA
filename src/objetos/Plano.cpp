#include "objetos/Plano.h"

// ============== CLASSE ==============

Plano::Plano(Ponto P, Vetor N, Propriedades propriedades, int m){
    this->P_pi = P;
    this->n = N;
    this->prop = propriedades;
    this->m = m;
}

// ============== MÉTODOS ASSOCIADOS AO PLANO ==============


bool IntersecaoRayPlano(Plano plano, Ray ray, float &ti){
    // t = - (w * n) / (dr * n)
    float denominador = produto_escalar(ray.dr, plano.n);

    // denominador prox. de zero -> raio paralelo ao plano
    if (fabs(denominador) < 1e-6) {
        return false;
    }

    // -w
    Vetor w = plano.P_pi - ray.P_0;
    // calculando parâmetro t de distância 
    float t = produto_escalar(w, plano.n) / denominador;
    // se t for positivo, a interseção é válida
    if(t > 0) {
        ti = t; 
        return true;}
    return false;
}

void Plano::setTextura(Textura* tex) {
    textura = tex;
    temTextura = (tex != nullptr);
}

Cor Plano::getCorTextura(const Ponto& Pt) const {
    if (!temTextura || textura == nullptr) {
        return prop.Kdif; // Retorna cor difusa padrão
    }
    
    // Calcular coordenadas de textura no plano
    // Assumindo que o plano está alinhado com eixos
    
    if (fabs(n.y) > 0.9f) { // Plano horizontal (chão/teto)
        float u = (Pt.x - P_pi.x) / 100.0f; // Escala
        float v = (Pt.z - P_pi.z) / 100.0f;
        return textura->amostrarRepetir(u, v);
    }
    else if (fabs(n.x) > 0.9f) { // Plano vertical em x
        float u = (Pt.y - P_pi.y) / 100.0f;
        float v = (Pt.z - P_pi.z) / 100.0f;
        return textura->amostrarRepetir(u, v);
    }
    else if (fabs(n.z) > 0.9f) { // Plano vertical em z
        float u = (Pt.x - P_pi.x) / 100.0f;
        float v = (Pt.y - P_pi.y) / 100.0f;
        return textura->amostrarRepetir(u, v);
    }
    
    return prop.Kdif; // Fallback
}