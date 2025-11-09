#include <bits/stdc++.h>
using namespace std;

// definição de um ponto genérico
typedef struct ponto{
    float cx, cy, cz;

    ponto(float x, float y, float z){
        this->cx = x;
        this->cy = y;
        this->cz = z;
    }
} ponto;

// definição de cor
typedef struct cor{
    float r, g, b;

    cor(float r = 0.0f, float g = 0.0f, float b = 0.0f){
        this->r = r;
        this->g = g;
        this->b = b;
    }

} cor;

// definição de um vetor genérico
typedef struct vetor{
    float cx,cy,cz;

    vetor(float x = 0.0f, float y = 0.0f, float z = 0.0f){
        this->cx = x;
        this->cy = y;
        this->cz = z;
    }

} vetor;


// ============================ FUNÇÕES AUXILIARES ============================
// mantém os valores de r,g,b entre 0 e 1
cor normalizaRGB(int r, int g, int b) {
    return cor(r / 255.0f, g / 255.0f, b / 255.0f);
}

// ponto2 - ponto1 = vetor entre ponto2 e ponto 1
vetor operator-(ponto ponto2, ponto ponto1){
    return vetor(ponto2.cx - ponto1.cx, ponto2.cy - ponto1.cy, ponto2.cz - ponto1.cz);
}

// inverter sentido de vetor 
vetor operator-(vetor v)
{
    return vetor(-v.cx, -v.cy, -v.cz);
}

// produto escalar entre dois vetores
float produto_escalar(vetor vetot1, vetor vetot2)
{
    return (vetot1.cx * vetot2.cx + vetot1.cy * vetot2.cy + vetot1.cz * vetot2.cz);
}

// ||v|| = sqrt(x^2 + y^2 + z^2)
// normalizacao: v/||v||
vetor normalizar(vetor v)
{
    float modulo = sqrtf(v.cx * v.cx + v.cy * v.cy + v.cz * v.cz);

    return vetor(v.cx / modulo, v.cy / modulo, v.cz / modulo);
}

cor arroba(cor a, cor b)
{
    return {a.r * b.r, a.g * b.g, a.b * b.b};
}
// ============================ VARIÁVEIS ============================

// janela
float wJanela = 60.0f;
float hJanela = 60.0f; 
float dJanela = 30.0f;
ponto centro_da_janela(0.0f,0.0f,-dJanela);

// olho do pintor
ponto olho_do_pintor(0.0f,0.0f,0.0f);

// ============= esfera =============

float rEsfera = 40.0f;
ponto centro_da_esfera(0.0f,0.0f,-100.0f); 
cor esfColor = normalizaRGB(255, 0, 0);

// propriedades da esfera 
cor Kdif_esfera(0.8f, 0.1f, 0.1f);    
cor Kesp_esfera(0.9f, 0.9f, 0.9f);    
cor Kamb_esfera(0.7f, 0.1f, 0.1f);    
int m_esfera = 50;   

// ============= plano do chão =============

// ponto conhecido do plano
ponto P_chao (0.0f, -rEsfera, 0.0f);
// vetor unitário normal ao plano
vetor n_chao (0, 1, 0);
// Reflectividade do plano do chão
cor Kdif_chao(0.2f, 0.7f, 0.2f);
cor Kesp_chao(0.0f, 0.0f, 0.0f);
cor Kamb_chao(0.2f, 0.7f, 0.2f);
int m_chao = 1;

// ============= plano de fundo =============

// ponto conhecido do plano
ponto P_fundo(0.0f, 0.0f, -200.0f);  
// vetor unitário normal ao plano
vetor n_fundo(0.0f, 0.0f, 1.0f);     
// Reflectividade do plano do fundo
cor Kdif_fundo(0.3f, 0.3f, 0.7f);
cor Kesp_fundo(0.0f, 0.0f, 0.0f);
cor Kamb_fundo(0.3f, 0.3f, 0.7f);
int m_fundo = 1;

// =========================================

// canvas
size_t nCol = 500;
size_t nLin = 500;

// ============= Iluminação =============

// intensidade da fonte
cor Intensidade_da_fonte (0.7f, 0.7f, 0.7f);
// intensidade da luz ambiente da cena
cor Intensidade_ambiente_da_cena (0.3f, 0.3f, 0.3f);
// ponto da fonte
ponto ponto_da_fonte (0.0f, 60.0f, -30.0f);

// ============================ FUNÇÕES DE INTERSEÇÃO ============================
bool Intersecao_raio_e_esfera(vetor dr, ponto origem, float &ti){
    // vetor que vai da olho_do_pintor ao centro da esfera
    vetor w = origem - centro_da_esfera; 

    // coeficientes da equação da interseção
    float a = produto_escalar(dr, dr);
    float b = 2 * (produto_escalar(w, dr));
    float c2 = produto_escalar(w, w) - rEsfera * rEsfera;

    double delta = b*b - 4*a*c2;

    // se delta negativo, então o raio não intercepta a esfera
    if(delta < 0) return false;

    // calculando parâmetros de distância t1 e t2
    float t1 = (-b + sqrt(delta)) / (2 * a);
    float t2 = (-b - sqrt(delta)) / (2 * a);

    // os pontos de interseção estão na frente do olho
    if (t1 > 0 && t2 > 0) ti = min(t1, t2); // escolhe a mais próxima   
    // apenas P(t1) está na frente do olho
    else if (t1 > 0) ti = t1; 
    // apenas P(t2) está na frente do olho  
    else if (t2 > 0) ti = t2;
    // nem P(t1) nem P(t2) são observáveis   
    else return false; 
    // delta é não negativo e pelo menos um ponto de interseção é observável
    return true;
}

bool Intersecao_raio_e_plano(vetor dr, ponto origem, ponto P_pi, float &ti, vetor n){
    // t = - (w * n) / (dr * n)
    float denominador = produto_escalar(dr, n);

    // denominador prox. de zero -> raio paralelo ao plano
    if (fabs(denominador) < 1e-6) {
        return false;
    }

    // -w
    vetor w = P_pi - origem;
    // calculando parâmetro t de distância 
    float t = produto_escalar(w, n) / denominador;
    // se t for positivo, a interseção é válida
    if(t > 0) {
        ti = t; 
        return true;}
    return false;
}

bool Sombra_no_plano(ponto Pt, int objeto = -1) {
    // vetor na direção da fonte
    vetor L = ponto_da_fonte - Pt;
    // distância da fonte até o ponto de interseção
    float distancia = sqrtf(produto_escalar(L, L));
    // normalizando L
    vetor l = normalizar(L);
    
    // deslocar origem para evitar self-intersection
    ponto origem_deslocada (Pt.cx + l.cx * 1e-6f,
                            Pt.cy + l.cy * 1e-6f,
                            Pt.cz + l.cz * 1e-6f);

    float ti_esfera, ti_chao, ti_fundo;
    
    // se P(t) não for ponto da esfera, verificar se o raio de P(t) até a 
// fonte intercepta a esfera. Se sim, a esfera é um obstáculo e P(t) é um ponto de sombra
    if (objeto != 0 && Intersecao_raio_e_esfera(l, origem_deslocada, ti_esfera)) {
        if (ti_esfera > 0 && ti_esfera < distancia) return true;
    }
    
    // se P(t) não for ponto do chão, verificar se o raio de P(t) até a 
// fonte intercepta o chão. Se sim, o chão é um obstáculo e P(t) é um ponto de sombra 
    if (objeto != 1 && Intersecao_raio_e_plano(l, origem_deslocada, P_chao, ti_chao, n_chao)) {
        if (ti_chao > 0 && ti_chao < distancia) return true;
    }
    
    // se P(t) não for ponto do fundo, verificar se o raio de P(t) até a 
// fonte intercepta o fundo. Se sim, o fundo é um obstáculo e P(t) é um ponto de sombra 
    if (objeto != 2 && Intersecao_raio_e_plano(l, origem_deslocada, P_fundo, ti_fundo, n_fundo)) {
        if (ti_fundo > 0 && ti_fundo < distancia) return true;
    }
    
    return false;
}

cor CorResultante(ponto Pt, vetor n, vetor dr, cor Kdif, cor Kesp, cor Kamb, int m, int objeto) {
    cor Intensidade_ambiente_refletida = arroba(Kamb, Intensidade_ambiente_da_cena);
    
    // verificar se ponto está em sombra
    if (Sombra_no_plano(Pt, objeto)) {
        return Intensidade_ambiente_refletida;  
    }
    
    // vetor na direção da fonte
    vetor l = normalizar(ponto_da_fonte - Pt);
    // vetor viwer na direção do observador
    vetor v = -dr;
    // Fd = l · n = cos(θ), e θ < 90°
    float Fd = max(0.0f, produto_escalar(l, n));

    // ========== I_dif = IF @ Kdif * Fd ==========
    // I_dif = IF @ Kdif
    cor Intensidade_difusa = arroba(Intensidade_da_fonte, Kdif);
    // I_dif = I_dif * Fd
    Intensidade_difusa.r = Intensidade_difusa.r * Fd;
    Intensidade_difusa.g = Intensidade_difusa.g * Fd;
    Intensidade_difusa.b = Intensidade_difusa.b * Fd;
    
    // apenas calcular especular se Kesp não for zero
    cor Intensidade_especular(0.0f, 0.0f, 0.0f);
    if (Kesp.r > 0 || Kesp.g > 0 || Kesp.b > 0) {
        // vetor reflexão 
        vetor r = normalizar(vetor( 2 * Fd * n.cx - l.cx, 
                                    2 * Fd * n.cy - l.cy, 
                                    2 * Fd * n.cz - l.cz ));
        
        // Fe = v · r = cos(θ), e θ < 90°
        float Fe_parcial = max(0.0f, produto_escalar(v, r));

        // ========== I_esp = IF @ Kesp * Fe ==========
        // I_esp = IF @ Kesp
        Intensidade_especular = arroba(Intensidade_da_fonte, Kesp);
        // Fe = (v * r)^m
        float Fe = pow(Fe_parcial, m);
        // I_esp = I_esp * Fe
        Intensidade_especular.r = Intensidade_especular.r * Fe;
        Intensidade_especular.g = Intensidade_especular.g * Fe;
        Intensidade_especular.b = Intensidade_especular.b * Fe;
    }
    
    // Internsidade que chega ao observador
    cor Intensidade_observada;
    Intensidade_observada.r = min(1.0f, Intensidade_ambiente_refletida.r + Intensidade_difusa.r + Intensidade_especular.r);
    Intensidade_observada.g = min(1.0f, Intensidade_ambiente_refletida.g + Intensidade_difusa.g + Intensidade_especular.g);
    Intensidade_observada.b = min(1.0f, Intensidade_ambiente_refletida.b + Intensidade_difusa.b + Intensidade_especular.b);
    
    return Intensidade_observada;
}

int main(){
    
    // matriz do canvas
    vector<vector<cor>> canvas(nLin, vector<cor>(nCol));

    // dimensões dos retângulos da tela de mosquito
    float Dx = wJanela/nCol;
    float Dy = hJanela/nLin;

    // laço principal que percorre a matriz do canvas
    for (size_t l = 0; l < nLin; l++){
        for (size_t c = 0; c < nCol; c++){
            
            // coordenadas do ponto do centro de um retângulo
            ponto centro_do_pixel(  -wJanela/2 + Dx/2  + c*Dx,
                                    hJanela/2 - Dy/2 - l*Dy,
                                    -dJanela  );
            
            // vetor raio
            vetor ray = centro_do_pixel - olho_do_pintor;
            // raio normalizado
            vetor dr = normalizar(ray);

            // parâmetro de distância do ponto de interseção P(t)
            float ti_esfera, ti_chao, ti_fundo;
            
            // interseções nos objetos
            bool hit_esfera = Intersecao_raio_e_esfera(dr, olho_do_pintor, ti_esfera);
            bool hit_chao = Intersecao_raio_e_plano(dr, olho_do_pintor, P_chao, ti_chao, n_chao );
            bool hit_fundo = Intersecao_raio_e_plano(dr, olho_do_pintor, P_fundo, ti_fundo, n_fundo);
            
            // encontrar a interseção mais próxima
            float ti_min = -1.0f; // ti inicialmente inválido
            int objeto_mais_proximo = -1; // -1: nenhum, 0: esfera, 1: chão, 2: fundo
            
            // verificando em qual objeto o raio bateu primeiro
            if (hit_esfera && ti_esfera > 0 && (ti_min < 0 || ti_esfera < ti_min)) {
                // se bateu na esfera com um ti válido e ti_esfera é o menor ti positivo, então
                ti_min = ti_esfera;
                objeto_mais_proximo = 0;
            }
            if (hit_chao && ti_chao > 0 && (ti_min < 0 || ti_chao < ti_min)) {
                ti_min = ti_chao;
                objeto_mais_proximo = 1;
            }
            if (hit_fundo && ti_fundo > 0 && (ti_min < 0 || ti_fundo < ti_min)) {
                ti_min = ti_fundo;
                objeto_mais_proximo = 2;
            }

            if (objeto_mais_proximo != -1) {
                ponto Pt (  olho_do_pintor.cx + ti_min * dr.cx, 
                            olho_do_pintor.cy + ti_min * dr.cy, 
                            olho_do_pintor.cz + ti_min * dr.cz  );
                
                // propriedades do objeto
                vetor normal;
                cor Kdif, Kesp, Kamb;
                int m;
                
                switch (objeto_mais_proximo) {
                    case 0: // esfera
                        normal = normalizar(Pt - centro_da_esfera);
                        Kdif = Kdif_esfera; Kesp = Kesp_esfera; Kamb = Kamb_esfera;
                        m = m_esfera;
                        break;
                    case 1: // chão
                        normal = n_chao;
                        Kdif = Kdif_chao; Kesp = Kesp_chao; Kamb = Kamb_chao;
                        m = m_chao;
                        break;
                    case 2: // fundo
                        normal = n_fundo;
                        Kdif = Kdif_fundo; Kesp = Kesp_fundo; Kamb = Kamb_fundo;
                        m = m_fundo;
                        break;
                }
                canvas[l][c] = CorResultante(Pt, normal, dr, Kdif, Kesp, Kamb, m, objeto_mais_proximo);

            }
        }
    }

    // gerar aquivo ppm
    ofstream arquivo("esfera_iluminada_com_planos.ppm");
    arquivo << "P3\n" << nCol << " " << nLin << "\n255\n";
    for (size_t l = 0; l < nLin; l++)
    {
        for (size_t c = 0; c < nCol; c++)
        {
            arquivo << canvas[l][c].r * 255 << " " << canvas[l][c].g * 255 << " " << canvas[l][c].b * 255 << " ";
        }
        arquivo << "\n";
    }
    arquivo.close();
    cout << "Imagem gerada." << endl;

    return 0;
}