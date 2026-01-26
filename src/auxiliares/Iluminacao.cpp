#include "auxiliares/Iluminacao.h"
using namespace std;

// ============== CLASSE ==============

// definição de Cor RGB
Cor::Cor(float r, float g, float b){
    this->r = r;
    this->g = g;
    this->b = b;
}

// definição das propriedades de um material
Propriedades::Propriedades(Cor Kd, Cor Ke, Cor Ka){
    this->Kdif = Kd;
    this->Kesp = Ke;
    this->Kamb = Ka;
}

IluminacaoCena::IluminacaoCena(Cor IF, Cor IA, Ponto PF){
    this->intensidade_da_fonte = IF;
    this->intensidade_ambiente = IA;
    this->ponto_da_fonte = PF;
}

// ============== MÉTODOS ASSOCIADOS A COR E ILUMINAÇÃO ==============

// mantém os valores de r,g,b entre 0 e 1
Cor normalizaRGB(int r, int g, int b) {
    return Cor(r / 255.0f, g / 255.0f, b / 255.0f);
}

Cor arroba(Cor A, Cor B){
    return Cor(A.r * B.r, A.g * B.g, A.b * B.b);
}

// Pt -> ponto atingido pelo ray
// n -> vetor normal do ponto atingido
Cor CorResultante(Ponto Pt, Vetor n, Ray ray, Propriedades prop,
    IluminacaoCena iluminacao, int m, int objeto) {

    Cor Intensidade_ambiente_refletida = arroba(prop.Kamb, iluminacao.intensidade_ambiente);
    Vetor dr = ray.dr;
    // verificar se Ponto está em sombra
    
    // Vetor na direção da fonte
    Vetor l = normalizar(iluminacao.ponto_da_fonte - Pt);
    // Vetor viwer na direção do observador
    Vetor v = -dr;
    // Fd = l · n = cos(θ), e θ < 90°
    float Fd = max(0.0f, produto_escalar(l, n));

    // ========== I_dif = IF @ Kdif * Fd ==========
    // I_dif = IF @ Kdif
    Cor Intensidade_difusa = arroba(iluminacao.intensidade_da_fonte, prop.Kdif);
    // I_dif = I_dif * Fd
    Intensidade_difusa.r = Intensidade_difusa.r * Fd;
    Intensidade_difusa.g = Intensidade_difusa.g * Fd;
    Intensidade_difusa.b = Intensidade_difusa.b * Fd;
    
    // apenas calcular especular se Kesp não for zero
    Cor Intensidade_especular(0.0f, 0.0f, 0.0f);
    if (prop.Kesp.r > 0 || prop.Kesp.g > 0 || prop.Kesp.b > 0) {
        // Vetor reflexão 
        Vetor r = normalizar(Vetor( 2 * Fd * n.x - l.x, 
                                    2 * Fd * n.y - l.y, 
                                    2 * Fd * n.z - l.z ));
        
        // Fe = v · r = cos(θ), e θ < 90°
        float Fe_parcial = max(0.0f, produto_escalar(v, r));

        // ========== I_esp = IF @ Kesp * Fe ==========
        // I_esp = IF @ Kesp
        Intensidade_especular = arroba(iluminacao.intensidade_da_fonte, prop.Kesp);
        // Fe = (v * r)^m
        float Fe = pow(Fe_parcial, m);
        // I_esp = I_esp * Fe
        Intensidade_especular.r = Intensidade_especular.r * Fe;
        Intensidade_especular.g = Intensidade_especular.g * Fe;
        Intensidade_especular.b = Intensidade_especular.b * Fe;
    }
    
    // Internsidade que chega ao observador
    Cor Intensidade_observada;
    Intensidade_observada.r = min(1.0f, Intensidade_ambiente_refletida.r + Intensidade_difusa.r + Intensidade_especular.r);
    Intensidade_observada.g = min(1.0f, Intensidade_ambiente_refletida.g + Intensidade_difusa.g + Intensidade_especular.g);
    Intensidade_observada.b = min(1.0f, Intensidade_ambiente_refletida.b + Intensidade_difusa.b + Intensidade_especular.b);
    
    return Intensidade_observada;
}

Cor operator*(Cor cor, float X){
    return Cor(cor.r * X, cor.g * X, cor.b * X);
}

Cor operator*(float X, Cor cor){
    return Cor(cor.r * X, cor.g * X, cor.b * X);
}
