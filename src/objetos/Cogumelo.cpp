#include "objetos/Cogumelo.h"

Cogumelo::Cogumelo(Ponto centroConexao, float raioCaule, float raioChapeu, int m) {
    
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

    SemiEsfera* caule = new SemiEsfera(
        centroConexao, 
        raioCaule, 
        Vetor(0, -1, 0), 
        propBege, 
        m, 
        true
    );
    adicionarComponente(caule);

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