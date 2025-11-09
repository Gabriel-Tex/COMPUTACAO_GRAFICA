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
vetor subtracao(ponto ponto2, ponto ponto1){
    return vetor(ponto2.cx - ponto1.cx, ponto2.cy - ponto1.cy, ponto2.cz - ponto1.cz);
}

// produto escalar entre dois vetores
float produto_escalar(vetor vetor1, vetor vetor2)
{
    return (vetor1.cx * vetor2.cx + vetor1.cy * vetor2.cy + vetor1.cz * vetor2.cz);
}

// ||v|| = sqrt(x^2 + y^2 + z^2)
// normalizacao: v/||v||
vetor normalizar(vetor v)
{
    float modulo = sqrtf(v.cx * v.cx + v.cy * v.cy + v.cz * v.cz);

    return vetor(v.cx / modulo, v.cy / modulo, v.cz / modulo);
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

// vetor que vai da origem ao centro da esfera
vetor w = subtracao(centro_da_esfera, olho_do_pintor); 
// ==================================

// background
cor bgColor = normalizaRGB(100,100,100);

// canvas
size_t nCol = 500;
size_t nLin = 500;

// ============================ FUNÇÕES DE INTERSEÇÃO ============================
bool intersecao_raio_e_esfera(vetor dr){

    // coeficientes da equação da interseção
    float a = produto_escalar(dr, dr);
    float b = 2 * (produto_escalar(w, dr));
    float c2 = produto_escalar(w, w) - rEsfera * rEsfera;

    double delta = b*b - 4*a*c2;

    // se delta positivo, então o raio intercepta a esfera
    if(delta >= 0) return true;
    return false;

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
            float x = - wJanela/2 + Dx/2  + c*Dx;
            float y = hJanela/2 - Dy/2 - l*Dy;
            float z = -dJanela;
            ponto centro_do_pixel(x,y,z);
            
            // vetor raio
            vetor ray = subtracao(centro_do_pixel, olho_do_pintor);
            vetor dr = normalizar(ray);

            // se delta >= 0, houve interseção; se delta < 0, não houve.
            if (intersecao_raio_e_esfera(dr)) {
                canvas[l][c] = esfColor;   
            } else {
                canvas[l][c] = bgColor;   
            }

        }
    }

    // gerar aquivo ppm
    ofstream arquivo("esfera.ppm");
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