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
float wJanela = 2;
float hJanela = 2; 
float dJanela = 1;
ponto centro_da_janela(0.0f,0.0f,-dJanela);

// olho do pintor
ponto olho_do_pintor(0.0f,0.0f,0.0f);

// ============= esfera =============
float rEsfera = 0.5f;
ponto centro_da_esfera(0.0f,0.0f,-(dJanela + rEsfera)); 
cor esfColor = normalizaRGB(255, 0, 0);
 
// propriedades da esfera 
cor Kdif(0.8f, 0.1f, 0.1f);    
cor Kesp(0.9f, 0.9f, 0.9f);    
cor Kamb(0.7f, 0.1f, 0.1f);    
int m = 50;                     
// ==================================

// background
cor bgColor = normalizaRGB(100,100,100);

// canvas
size_t nCol = 500;
size_t nLin = 500;

// ============= Iluminação =============
// intensidade da fonte
cor Intensidade_da_fonte (0.7f, 0.7f, 0.7f);
// intensidade da luz ambiente da cena
cor Intensidade_ambiente_da_cena (0.3f, 0.3f, 0.3f);
// ponto da fonte
ponto ponto_da_fonte (0.0f, 5.0f, 0.0f);

//I_d =( I_F @ K)* (l . n)
//I_e = (I_F @ K)*((v . r)**m)

//I_F = (0.7, 0.7, 0.7) // intensidade da fonte pontual
//P_F = (0, 5, 0) // Posição da fonte pontual situada a 5 metros acima do olho do observador.

// ============================ FUNÇÕES DE INTERSEÇÃO ============================
bool intersecao_raio_e_esfera(vetor dr, float &ti){
    // vetor que vai da olho_do_pintor ao centro da esfera
    vetor w = olho_do_pintor - centro_da_esfera; 

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

            // parâmetro de distância do ponto de interseção P(t)
            float ti;
            // cor final do pixel (se nao houver interseção com a esfera, é cinza)
            cor CorResultante = bgColor;
            
            // vetor raio
            vetor ray = centro_do_pixel - olho_do_pintor;
            // raio normalizado
            vetor dr = normalizar(ray);

            // se delta >= 0, houve interseção; se delta < 0, não houve.
            if (intersecao_raio_e_esfera(dr, ti)) {
                
                // ponto de interseção do raio com a esfera -> P(t) = P0 + t * dr
                ponto Pt (  olho_do_pintor.cx + ti * dr.cx, 
                            olho_do_pintor.cy + ti * dr.cy, 
                            olho_do_pintor.cz + ti * dr.cz  );

                // normal do ponto de interseção 
                vetor n = normalizar(Pt - centro_da_esfera);

                // vetor na direção da fonte
                vetor l = normalizar(ponto_da_fonte - Pt);

                // vetor viwer na direção do observador
                vetor v = -dr;
                
                // Fd = l · n = cos(θ), e θ < 90°
                float Fd = max(0.0f, produto_escalar(l, n));

                // vetor reflexão 
                vetor r = normalizar(vetor( 2 * Fd * n.cx - l.cx, 
                                            2 * Fd * n.cy - l.cy, 
                                            2 * Fd * n.cz - l.cz ));

                // Fe = v · r = cos(θ), e θ < 90°
                float Fe_parcial = max(0.0f, produto_escalar(v, r));
                
                // ========== I_dif = IF @ Kdif * Fd ==========
                // I_dif = IF @ Kdif
                cor Intensidade_difusa = arroba(Intensidade_da_fonte, Kdif);
                // I_dif = I_dif * Fd
                Intensidade_difusa.r = Intensidade_difusa.r * Fd;
                Intensidade_difusa.g = Intensidade_difusa.g * Fd;
                Intensidade_difusa.b = Intensidade_difusa.b * Fd;
                
                // ========== I_esp = IF @ Kesp * Fe ==========
                // I_esp = IF @ Kesp
                cor Intensidade_especular = arroba(Intensidade_da_fonte, Kesp);
                
                // Fe = (v * r)^m
                float Fe = pow(Fe_parcial, m);
                // I_esp = I_esp * Fe
                Intensidade_especular.r = Intensidade_especular.r * Fe;
                Intensidade_especular.g = Intensidade_especular.g * Fe;
                Intensidade_especular.b = Intensidade_especular.b * Fe;
                
                // ========== I_amb_ref = I_amb_cena @ Kamb ==========
                cor Intensidade_ambiente_refletida = arroba(Kamb, Intensidade_ambiente_da_cena);

                CorResultante.r = min(1.0f, Intensidade_difusa.r + Intensidade_especular.r + Intensidade_ambiente_refletida.r);
                CorResultante.g = min(1.0f, Intensidade_difusa.g + Intensidade_especular.g + Intensidade_ambiente_refletida.g);
                CorResultante.b = min(1.0f, Intensidade_difusa.b + Intensidade_especular.b + Intensidade_ambiente_refletida.b);
            } 

        canvas[l][c] = CorResultante;
        
        }
        
    }

    // gerar aquivo ppm
    ofstream arquivo("esfera_iluminada.ppm");
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