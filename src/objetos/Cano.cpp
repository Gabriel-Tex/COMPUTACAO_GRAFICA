#include "objetos/Cano.h"

Cano::Cano(Ponto baseCentral, float raioCorpo, float alturaCorpo, 
           float raioBorda, float alturaBorda, Vetor direcao, 
           Propriedades prop, int m) {
    
    Vetor d = normalizar(direcao);

    Cilindro* corpo = new Cilindro(baseCentral, raioCorpo, alturaCorpo, d, prop, m, true, false);
    adicionarComponente(corpo);

    Ponto inicioBorda = baseCentral + (d * (alturaCorpo + 0.001f));
    
    Cilindro* borda = new Cilindro(inicioBorda, raioBorda, alturaBorda, d, prop, m, true, false);
    adicionarComponente(borda);

    this->props = prop;
    this->materialId = m;
}