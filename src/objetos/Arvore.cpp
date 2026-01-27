#include "objetos/Arvore.h"

Arvore::Arvore(Ponto baseTronco, float alturaTronco, float raioTronco, float raioCopa, int m)
    : baseTronco(baseTronco), alturaTronco(alturaTronco), raioTronco(raioTronco), raioCopa(raioCopa), m(m) {
    
    Propriedades propMarrom(Cor(0.4f, 0.2f, 0.1f), Cor(0.1f, 0.1f, 0.1f), Cor(0.05f, 0.05f, 0.05f));
    Propriedades propVerde(Cor(0.1f, 0.4f, 0.1f), Cor(0.2f, 0.2f, 0.2f), Cor(0.05f, 0.1f, 0.05f));

    Cilindro* tronco = new Cilindro(
        baseTronco, 
        raioTronco, 
        alturaTronco, 
        Vetor(0, 1, 0), 
        propMarrom, 
        m, 
        true,  // temBase
        false  // temTopo
    );
    adicionarComponente(tronco);

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

void Arvore::transforma(const Matriz4x4& M) {
    // Atualiza o ponto de referência global da árvore
    this->baseTronco = M * this->baseTronco;
    
    // Propaga a transformação para todos os componentes (Cilindro e SemiEsfera)
    for (auto comp : componentes) {
        comp->transforma(M);
    }
}

void Arvore::transladar(float tx, float ty, float tz) {
    Matriz4x4 T = Transformacao::translacao(tx, ty, tz);
    this->transforma(T);
}

void Arvore::escalar(float s) {
    // Usamos a base do tronco como ponto fixo para que a árvore cresça a partir do chão
    // sem "voar" ou afundar.
    Matriz4x4 S = Transformacao::escala(s, s, s, baseTronco);
    this->transforma(S);
}

void Arvore::rotacionarX(float anguloGraus) {
    // Reutiliza a lógica de ObjetoComplexo usando a baseTronco como pivô
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