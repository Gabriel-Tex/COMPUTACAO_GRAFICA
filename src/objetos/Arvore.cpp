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