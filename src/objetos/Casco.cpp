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