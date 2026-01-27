#include "objetos/Casco.h"

Casco::Casco(Ponto centroBase, float raioCasco, int m)
    : centroBase(centroBase), raioCasco(raioCasco), m(m) {

    Propriedades propVermelho(Cor(0.8f, 0.05f, 0.05f), Cor(0.3f, 0.1f, 0.1f), Cor(0.1f, 0.05f, 0.05f));
    Propriedades propCinza(Cor(0.6f, 0.6f, 0.6f), Cor(0.4f, 0.4f, 0.4f), Cor(0.2f, 0.2f, 0.2f));

    SemiEsfera* casco = new SemiEsfera(
        centroBase, 
        raioCasco, 
        Vetor(0, 1, 0), 
        propVermelho, 
        m, 
        m,    
        true
    );
    adicionarComponente(casco);

    float alturaEspinho = raioCasco * 0.4f;
    float raioBaseEspinho = raioCasco * 0.15f;

    Ponto topo = centroBase + Vetor(0, raioCasco, 0);
    adicionarComponente(new Cone(topo, raioBaseEspinho, alturaEspinho, Vetor(0, 1, 0), propCinza, m));

    float anguloInclinacao = M_PI / 4.0f; 
    float r_horizontal = raioCasco * sin(anguloInclinacao);
    float y_local = raioCasco * cos(anguloInclinacao);

    for (int i = 0; i < 4; i++) {
        float theta = i * (M_PI / 2.0f); 
        
        Vetor dirEspinho(cos(theta) * sin(anguloInclinacao), cos(anguloInclinacao), sin(theta) * sin(anguloInclinacao));
        Ponto posEspinho = centroBase + (dirEspinho * raioCasco);

        adicionarComponente(new Cone(
            posEspinho, 
            raioBaseEspinho, 
            alturaEspinho, 
            dirEspinho, 
            propCinza, 
            m
        ));
    }

    this->m = m;
}


void Casco::transforma(const Matriz4x4& M) {
    this->centroBase = M * this->centroBase;
    
    ObjetoComplexo::transforma(M);
}

void Casco::transladar(float tx, float ty, float tz) {
    Matriz4x4 T = Transformacao::translacao(tx, ty, tz);
    this->transforma(T);
}

void Casco::escalar(float s) {
    Matriz4x4 M = Transformacao::escala(s, s, s, centroBase);
    this->transforma(M);
}

void Casco::rotacionarX(float anguloGraus) {
    ObjetoComplexo::rotacionarX(anguloGraus, centroBase);
}

void Casco::rotacionarY(float anguloGraus) {
    ObjetoComplexo::rotacionarY(anguloGraus, centroBase);
}

void Casco::rotacionarZ(float anguloGraus) {
    ObjetoComplexo::rotacionarZ(anguloGraus, centroBase);
}

void Casco::rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus) {
    ObjetoComplexo::rotacionarEmEixoArbitrario(eixo, anguloGraus, centroBase);
}

void Casco::espelharXY() {
    Matriz4x4 M = Transformacao::espelhamentoXY();
    this->transforma(M);
}

void Casco::espelharXZ() {
    Matriz4x4 M = Transformacao::espelhamentoXZ();
    this->transforma(M);
}

void Casco::espelharYZ() {
    Matriz4x4 M = Transformacao::espelhamentoYZ();
    this->transforma(M);
}