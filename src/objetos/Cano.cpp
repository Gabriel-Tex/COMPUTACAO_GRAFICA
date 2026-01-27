#include "objetos/Cano.h"

Cano::Cano(Ponto baseCentral, float raioCorpo, float alturaCorpo, 
           float raioBorda, float alturaBorda, Vetor direcao, 
           Propriedades prop, int m) {
    
    Vetor d = normalizar(direcao);

    this->eixoArmazenado = d;
    this->alturaArmazenada = alturaCorpo;

    Cilindro* corpo = new Cilindro(baseCentral, raioCorpo, alturaCorpo, d, prop, m, true, false);
    adicionarComponente(corpo);

    Ponto inicioBorda = baseCentral + (d * (alturaCorpo + 0.001f));
    
    Cilindro* borda = new Cilindro(inicioBorda, raioBorda, alturaBorda, d, prop, m, true, false);
    adicionarComponente(borda);

    this->props = prop;
    this->materialId = m;
}

void Cano::transforma(const Matriz4x4& M) {
    this->baseCentral = M * this->baseCentral;

    Vetor vetorCorpo = this->eixoArmazenado * this->alturaArmazenada;
    
    Vetor vetorTransformado = M * vetorCorpo; 

    this->alturaArmazenada = comprimento(vetorTransformado);

    this->eixoArmazenado = normalizar(vetorTransformado);
    
    for (auto comp : componentes) {
        comp->transforma(M);
    }
}

void Cano::transladar(float tx, float ty, float tz) {
    Matriz4x4 T = Transformacao::translacao(tx, ty, tz);
    this->transforma(T);
}

void Cano::escalar(float s) {
    Cilindro* corpo = (Cilindro*)componentes[0]; 

    Ponto pBase = corpo->getCentroBase(); 
    Ponto pTopo = corpo->calcularTopo();

    Ponto meioDoCano = (pBase + pTopo) * 0.5f;

    Matriz4x4 S = Transformacao::escala(s, s, s, meioDoCano);
    
    this->transforma(S);
}

void Cano::rotacionarX(float anguloGraus) {
    Cilindro* corpo = (Cilindro*)componentes[0];
    Ponto pBase = corpo->getCentroBase(); 
    Ponto pTopo = corpo->calcularTopo();
    Ponto meioDoCano = (pBase + pTopo) * 0.5f;

    ObjetoComplexo::rotacionarX(anguloGraus, meioDoCano);
}

void Cano::rotacionarY(float anguloGraus) {
    Cilindro* corpo = (Cilindro*)componentes[0];
    Ponto pBase = corpo->getCentroBase(); 
    Ponto pTopo = corpo->calcularTopo();
    Ponto meioDoCano = (pBase + pTopo) * 0.5f;

    ObjetoComplexo::rotacionarY(anguloGraus, meioDoCano);
}

void Cano::rotacionarZ(float anguloGraus) {
    Cilindro* corpo = (Cilindro*)componentes[0];
    Ponto pBase = corpo->getCentroBase(); 
    Ponto pTopo = corpo->calcularTopo();
    Ponto meioDoCano = (pBase + pTopo) * 0.5f;

    ObjetoComplexo::rotacionarZ(anguloGraus, meioDoCano);
}

void Cano::rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus) {
    Cilindro* corpo = (Cilindro*)componentes[0];
    Ponto pBase = corpo->getCentroBase(); 
    Ponto pTopo = corpo->calcularTopo();
    Ponto meioDoCano = (pBase + pTopo) * 0.5f;

    ObjetoComplexo::rotacionarEmEixoArbitrario(eixo, anguloGraus, meioDoCano);
}


void Cano::cisalharX_XZ(float anguloGraus) {
    ObjetoComplexo::cisalharX_XZ(anguloGraus, baseCentral);
}

void Cano::cisalharY_XZ(float anguloGraus) {
    ObjetoComplexo::cisalharY_XZ(anguloGraus, baseCentral);
}

void Cano::cisalharY_XY(float anguloGraus) {
    ObjetoComplexo::cisalharY_XY(anguloGraus, baseCentral);
}

void Cano::cisalharZ_XY(float anguloGraus) {
    ObjetoComplexo::cisalharZ_XY(anguloGraus, baseCentral);
}

void Cano::espelharXY() {
    Matriz4x4 M = Transformacao::espelhamentoXY();
    this->transforma(M);
}

void Cano::espelharXZ() {
    Matriz4x4 M = Transformacao::espelhamentoXZ();
    this->transforma(M);
}

void Cano::espelharYZ() {
    Matriz4x4 M = Transformacao::espelhamentoYZ();
    this->transforma(M);
}