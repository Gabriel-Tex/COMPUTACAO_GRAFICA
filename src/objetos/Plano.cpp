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
