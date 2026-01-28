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

Cor CorResultante(Ponto Pt, Vetor n, Ray ray, Propriedades prop,
    IluminacaoCena iluminacao, int m, int objeto) {

    Cor Intensidade_ambiente_refletida = arroba(prop.Kamb, iluminacao.intensidade_ambiente);
    Vetor dr = ray.dr;

    Vetor l;
    Cor IF = iluminacao.intensidade_da_fonte;

    if (iluminacao.tipo == DIRECIONAL) {
        // l = -df
        l = normalizar(iluminacao.direcao_da_fonte * -1.0f);
    } 
    else { 
        l = normalizar(iluminacao.ponto_da_fonte - Pt);
        
        if (iluminacao.tipo == SPOT) {
            // ângulo entre df e l
            float cos_ponto = produto_escalar(l * -1.0f, normalizar(iluminacao.direcao_da_fonte));
            // verifica se o cos de alfa é menor que o cos de theta. Se sim, alfa está
        // fora de teta (alfa maior que teta)
            if (cos_ponto < iluminacao.cos_theta) {
                IF = Cor(0, 0, 0); 
            }
        }
    }

    Vetor v = -dr;
    float Fd = max(0.0f, produto_escalar(l, n));

    // I_dif = IF @ Kdif * Fd
    Cor Intensidade_difusa = arroba(IF, prop.Kdif);
    Intensidade_difusa.r *= Fd;
    Intensidade_difusa.g *= Fd;
    Intensidade_difusa.b *= Fd;
    
    Cor Intensidade_especular(0.0f, 0.0f, 0.0f);
    if (prop.Kesp.r > 0 || prop.Kesp.g > 0 || prop.Kesp.b > 0) {
        Vetor r = normalizar(Vetor( 2 * Fd * n.x - l.x, 
                                    2 * Fd * n.y - l.y, 
                                    2 * Fd * n.z - l.z ));
        
        float Fe_parcial = max(0.0f, produto_escalar(v, r));
        float Fe = pow(Fe_parcial, m);

        Intensidade_especular = arroba(IF, prop.Kesp);
        Intensidade_especular.r *= Fe;
        Intensidade_especular.g *= Fe;
        Intensidade_especular.b *= Fe;
    }
    
    Cor Intensidade_observada;
    Intensidade_observada.r = min(1.0f, Intensidade_ambiente_refletida.r + Intensidade_difusa.r + Intensidade_especular.r);
    Intensidade_observada.g = min(1.0f, Intensidade_ambiente_refletida.g + Intensidade_difusa.g + Intensidade_especular.g);
    Intensidade_observada.b = min(1.0f, Intensidade_ambiente_refletida.b + Intensidade_difusa.b + Intensidade_especular.b);
    
    return Intensidade_observada;
}

void IluminacaoCena::setPontual(Ponto pos, Cor intensidade) {
    tipo = PONTUAL;
    ponto_da_fonte = pos;
    intensidade_da_fonte = intensidade;
}

void IluminacaoCena::setDirecional(Vetor dir, Cor intensidade) {
    tipo = DIRECIONAL;
    direcao_da_fonte = normalizar(dir);
    intensidade_da_fonte = intensidade;
}

void IluminacaoCena::setSpot(Ponto pos, Vetor dir, float anguloGraus, Cor intensidade) {
    tipo = SPOT;
    ponto_da_fonte = pos;
    direcao_da_fonte = normalizar(dir);
    intensidade_da_fonte = intensidade;
    cos_theta = cos(anguloGraus * M_PI / 180.0f);
}

Cor operator*(Cor cor, float X){
    return Cor(cor.r * X, cor.g * X, cor.b * X);
}

Cor operator*(float X, Cor cor){
    return Cor(cor.r * X, cor.g * X, cor.b * X);
}
