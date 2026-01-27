#include "objetos/Arvore.h"

Arvore::Arvore(Ponto baseTronco, float alturaTronco, float raioTronco, float raioCopa, int m)
    : baseTronco(baseTronco), alturaTronco(alturaTronco), raioTronco(raioTronco), raioCopa(raioCopa), m(m) {
    
        // materiais da árvore
    Propriedades propMarrom(Cor(0.4f, 0.2f, 0.1f), Cor(0.1f, 0.1f, 0.1f), Cor(0.05f, 0.05f, 0.05f));
    Propriedades propVerde(Cor(0.1f, 0.4f, 0.1f), Cor(0.2f, 0.2f, 0.2f), Cor(0.05f, 0.1f, 0.05f));

    // tronco da árvore - cilindro marrom com topo e base cujo eixo é paralelo ao eixo Y 
    Cilindro* tronco = new Cilindro(
        baseTronco, 
        raioTronco, 
        alturaTronco, 
        Vetor(0, 1, 0), 
        propMarrom, 
        m, 
        true,  
        true 
    );
    adicionarComponente(tronco);

    // copa da árvore começa no topo do tronco (ponto da base + altura)
    Ponto centroCopa = baseTronco + Vetor(0, alturaTronco, 0);
    
    SemiEsfera* copa = new SemiEsfera(
        centroCopa, 
        raioCopa, 
        Vetor(0, 1, 0), 
        propVerde, 
        m, 
        true
    );
    adicionarComponente(copa);
}

// ============= MATRIZES DE TRANSFORMAÇÃO =============

void Arvore::transforma(const Matriz4x4& M) {
    this->baseTronco = M * this->baseTronco;
    
    for (auto comp : componentes) {
        comp->transforma(M);
    }
}

void Arvore::transladar(float tx, float ty, float tz) {
    Matriz4x4 T = Transformacao::translacao(tx, ty, tz);
    this->transforma(T);
}

void Arvore::escalar(float s) {
    Matriz4x4 S = Transformacao::escala(s, s, s, baseTronco);
    this->transforma(S);
}

void Arvore::rotacionarX(float anguloGraus) {
    ObjetoComplexo::rotacionarX(anguloGraus, baseTronco);
}

void Arvore::rotacionarY(float anguloGraus) {
    ObjetoComplexo::rotacionarY(anguloGraus, baseTronco);
}

void Arvore::rotacionarZ(float anguloGraus) {
    ObjetoComplexo::rotacionarZ(anguloGraus, baseTronco);
}

void Arvore::rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus) {
    ObjetoComplexo::rotacionarEmEixoArbitrario(eixo, anguloGraus, baseTronco);
}

void Arvore::espelharXY() {
    Matriz4x4 M = Transformacao::espelhamentoXY();
    this->transforma(M);
}

void Arvore::espelharXZ() {
    Matriz4x4 M = Transformacao::espelhamentoXZ();
    this->transforma(M);
}

void Arvore::espelharYZ() {
    Matriz4x4 M = Transformacao::espelhamentoYZ();
    this->transforma(M);
}