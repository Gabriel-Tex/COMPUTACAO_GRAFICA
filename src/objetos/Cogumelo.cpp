#include "objetos/Cogumelo.h"

Cogumelo::Cogumelo(Ponto centroConexao, float raioCaule, float raioChapeu, int m) {
    
    this->centroBase = centroConexao;

    // materiais do cogumelo
    Propriedades propBege(
        Cor(0.9f, 0.8f, 0.6f), 
        Cor(0.2f, 0.2f, 0.2f), 
        Cor(0.1f, 0.1f, 0.1f)  
    );

    Propriedades propVermelho(
        Cor(0.8f, 0.0f, 0.0f), 
        Cor(0.6f, 0.6f, 0.6f), 
        Cor(0.1f, 0.0f, 0.0f)  
    );

    // semiesfera inferior
    SemiEsfera* caule = new SemiEsfera(
        centroConexao, 
        raioCaule, 
        Vetor(0, -1, 0), 
        propBege, 
        m, 
        true
    );
    adicionarComponente(caule);

    // semisfera superior
    SemiEsfera* chapeu = new SemiEsfera(
        centroConexao, 
        raioChapeu, 
        Vetor(0, 1, 0), 
        propVermelho, 
        m, 
        true
    );
    adicionarComponente(chapeu);

    this->props = propVermelho;
    this->m = m;
}

// ================ MATRIZES DE TRANSFORMAÇÃO ================

void Cogumelo::transforma(const Matriz4x4& M) {
    this->centroBase = M * this->centroBase;
    
    ObjetoComplexo::transforma(M);
}

void Cogumelo::transladar(float tx, float ty, float tz) {
    Matriz4x4 T = Transformacao::translacao(tx, ty, tz);

    this->transforma(T);
}

void Cogumelo::escalar(float s) {
    Matriz4x4 M = Transformacao::escala(s, s, s, centroBase);
    this->transforma(M);
}

void Cogumelo::rotacionarX(float anguloGraus) {
    ObjetoComplexo::rotacionarX(anguloGraus, centroBase);
}

void Cogumelo::rotacionarY(float anguloGraus) {
    ObjetoComplexo::rotacionarY(anguloGraus, centroBase);
}

void Cogumelo::rotacionarZ(float anguloGraus) {
    ObjetoComplexo::rotacionarZ(anguloGraus, centroBase);
}

void Cogumelo::rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus) {
    ObjetoComplexo::rotacionarEmEixoArbitrario(eixo, anguloGraus, centroBase);
}

void Cogumelo::espelharXY() {
    Matriz4x4 M = Transformacao::espelhamentoXY();
    this->transforma(M);
}

void Cogumelo::espelharXZ() {
    Matriz4x4 M = Transformacao::espelhamentoXZ();
    this->transforma(M);
}

void Cogumelo::espelharYZ() {
    Matriz4x4 M = Transformacao::espelhamentoYZ();
    this->transforma(M);
}