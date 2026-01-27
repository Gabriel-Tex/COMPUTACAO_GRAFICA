#include "objetos/Bandeira.h"

Bandeira::Bandeira(Ponto baseHaste, float alturaHaste, int m)
    : baseHaste(baseHaste), alturaHaste(alturaHaste), m(m) {

    float raioHaste = 2.0f;
    float raioEsfera = 3.0f;
    float larguraBandeira = 20.0f;
    float alturaBandeira = 20.0f;

    Propriedades propCinza(Cor(0.7f, 0.7f, 0.7f), Cor(0.5f, 0.5f, 0.5f), Cor(0.2f, 0.2f, 0.2f)); 
    Propriedades propDourado(Cor(0.8f, 0.6f, 0.2f), Cor(0.9f, 0.8f, 0.4f), Cor(0.3f, 0.2f, 0.1f)); 
    Propriedades propVermelho(Cor(0.9f, 0.0f, 0.0f), Cor(0.2f, 0.2f, 0.2f), Cor(0.1f, 0.0f, 0.0f)); 

    Cilindro* haste = new Cilindro(
        baseHaste, 
        raioHaste, 
        alturaHaste, 
        Vetor(0, 1, 0), 
        propCinza, 
        m, 
        true, 
        true  
    );
    adicionarComponente(haste);

   
    Ponto topoHaste = baseHaste + (Vetor(0, 1, 0) * alturaHaste);
    Esfera* ponteira = new Esfera(
        raioEsfera, 
        topoHaste, 
        propDourado.Kdif, 
        propDourado,     
        m
    );
    adicionarComponente(ponteira);

    Ponto A = topoHaste + Vetor(raioHaste, 0, 0); 
    Ponto B = topoHaste + Vetor(raioHaste, -alturaBandeira, 0);
    Ponto C = topoHaste + Vetor(raioHaste + larguraBandeira, -alturaBandeira / 2.0f, 0);

    Vetor nTri = normalizar(produto_vetorial(B - A, C - A));

    Triangulo* triangulo = new Triangulo(A, B, C, nTri, propVermelho, m);
    adicionarComponente(triangulo);

    this->m = m; 
}

void Bandeira::transforma(const Matriz4x4& M) {
    this->baseHaste = M * this->baseHaste;

    ObjetoComplexo::transforma(M); 
}

void Bandeira::transladar(float tx, float ty, float tz) {
    Matriz4x4 T = Transformacao::translacao(tx, ty, tz);
    this->transforma(T);
}

void Bandeira::escalar(float s) {
    Matriz4x4 S = Transformacao::escala(s, s, s, baseHaste);
    this->transforma(S);
}

void Bandeira::rotacionarX(float anguloGraus) {
    ObjetoComplexo::rotacionarX(anguloGraus, baseHaste);
}

void Bandeira::rotacionarY(float anguloGraus) {
    ObjetoComplexo::rotacionarY(anguloGraus, baseHaste);
}

void Bandeira::rotacionarZ(float anguloGraus) {
    ObjetoComplexo::rotacionarZ(anguloGraus, baseHaste);
}

void Bandeira::rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus) {
    ObjetoComplexo::rotacionarEmEixoArbitrario(eixo, anguloGraus, baseHaste);
}

void Bandeira::espelharXY() {
    Matriz4x4 M = Transformacao::espelhamentoXY();
    this->transforma(M);
}

void Bandeira::espelharXZ() {
    Matriz4x4 M = Transformacao::espelhamentoXZ();
    this->transforma(M);
}

void Bandeira::espelharYZ() {
    Matriz4x4 M = Transformacao::espelhamentoYZ();
    this->transforma(M);
}