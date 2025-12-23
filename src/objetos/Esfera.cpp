#include "objetos/Esfera.h"
using namespace std;

// ============== CLASSE ==============
Esfera::Esfera(float raio, Ponto centro, Cor cor, Propriedades propriedades, int m){
    this->rEsfera = raio;
    this->cEsfera = centro;
    this->propriedades_esfera = propriedades;
    this->m_esfera = m;
}

// ============== MÉTODOS ASSOCIADOS A ESFERA ==============

bool IntersecaoRayEsfera(Esfera esfera, Ray ray, float &ti){
    // vetor que vai da olho_do_pintor ao centro da esfera
    Vetor w = ray.P_0 - esfera.cEsfera;

    // coeficientes da equação da interseção
    float a = produto_escalar(ray.dr, ray.dr);
    float b = 2 * (produto_escalar(w, ray.dr));
    float c = produto_escalar(w, w) - esfera.rEsfera * esfera.rEsfera;

    double delta = b*b - 4*a*c;

    // se delta negativo, então o raio não intercepta a esfera
    if(delta < 0) return false;

    // calculando parâmetros de distância t1 e t2
    float t1 = (-b + sqrt(delta)) / (2 * a);
    float t2 = (-b - sqrt(delta)) / (2 * a);

    // os pontos de interseção estão na frente do olho
    if (t1 > 0 && t2 > 0) ti = min(t1, t2); // escolhe a mais próxima   
    // apenas P(t1) está na frente do olho
    else if (t1 > 0) ti = t1; 
    // apenas P(t2) está na frente do olho  
    else if (t2 > 0) ti = t2;
    // nem P(t1) nem P(t2) são observáveis   
    else return false; 
    // delta é não negativo e pelo menos um ponto de interseção é observável
    return true;
}