/*  Compilação: 
g++ -std=c++17 -Iinclude src/auxiliares/*.cpp src/objetos/*.cpp tarefas/tarefa03_v2.cpp 
-o output/tarefa03/tarefa03_v2.exe 
    Execução:
.\output\tarefa03\tarefa03_v2.exe
*/
// auxiliares
#include "auxiliares/Vetores.h"
#include "auxiliares/Pontos.h"
#include "auxiliares/Janela.h"
#include "auxiliares/Ray.h"
#include "auxiliares/Canvas.h"
#include "auxiliares/Iluminacao.h"
// objetos
#include "objetos/Esfera.h"
#include "objetos/Plano.h"
// libs
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace std;

int main(){
    // constante de correção
    const float EPS = 1e-4f;

    // =========== CANVAS ===========
    Canvas canvas(500,500);

    // =========== JANELA ===========
    Janela janela(60.0f, 60.0f, 30.0f);
    Ponto  centro_da_janela(0.0f, 0.0f, -janela.dJ);
    Ponto  olho_do_pintor(0.0f,0.0f,0.0f);

    // =========== ILUMINAÇÃO DA CENA ===========
    Cor   IF(0.7f, 0.7f, 0.7f);
    Cor   IA(0.3f, 0.3f, 0.3f);
    Ponto PF(0.0f, 60.0f, -30.0f);

    IluminacaoCena iluminacao(IF, IA, PF);

    // =========== PARÂMETRO DOS OBJETOS ===========
        //  ESFERA01
        float rEsfera01 = 40.0f;
        Ponto cEsfera01(0.0f,0.0f,-100.0f);
        Cor esfColor01(normalizaRGB(255, 0, 0));
            Cor Kdif_esf01(0.8f, 0.1f, 0.1f);    
            Cor Kesp_esf01(0.9f, 0.9f, 0.9f);    
            Cor Kamb_esf01(0.7f, 0.1f, 0.1f);    
        int m_esf01 = 50;   
        Propriedades propEsf01(Kdif_esf01, Kesp_esf01, Kamb_esf01);

        // PLANO_CHAO
        Ponto P_chao(0.0f, -rEsfera01, 0.0f);
        Vetor n_chao (0, 1, 0);
            Cor Kdif_chao(0.2f, 0.7f, 0.2f);
            Cor Kesp_chao(0.0f, 0.0f, 0.0f);
            Cor Kamb_chao(0.2f, 0.7f, 0.2f);
        int m_chao = 1;
        Propriedades propPlano_chao(Kdif_chao, Kesp_chao, Kamb_chao);

        // PLANO_FUNDO
        Ponto P_fundo(0.0f, 0.0f, -200.0f);
        Vetor n_fundo(0.0f, 0.0f, 1.0f); 
            Cor Kdif_fundo(0.3f, 0.3f, 0.7f);
            Cor Kesp_fundo(0.0f, 0.0f, 0.0f);
            Cor Kamb_fundo(0.3f, 0.3f, 0.7f);
        int m_fundo = 1;
        Propriedades propPlano_fundo(Kdif_fundo, Kesp_fundo, Kamb_fundo);

    // =========== OBJETOS ===========
    // ESFERA01
    Esfera esfera01(rEsfera01, cEsfera01, esfColor01,
    propEsf01, m_esf01);
    // PLANO_CHAO
    Plano plano_chao(P_chao, n_chao, propPlano_chao, m_chao);
    // PLANO_FUNDO
    Plano plano_fundo(P_fundo, n_fundo, propPlano_fundo, m_fundo);
    
    // =========== RENDERIZAÇÃO ===========

    // matriz do canvas
    vector<vector<Cor>> canvasArray(canvas.nLin, vector<Cor>(canvas.nCol, canvas.bgColor));
    
    // dimensões dos retângulos da tela de mosquito
    float Dx = janela.wJ/canvas.nCol;
    float Dy = janela.hJ/canvas.nLin;

    Ponto Plc; // Plc -> ponto do centro do pixel
    Plc.z = -janela.dJ;
    // laço principal que percorre a matriz do canvas
    for (size_t L = 0; L < canvas.nLin; L++){
        Plc.y = janela.hJ/2 - Dy/2 - L*Dy;

        for (size_t C = 0; C < canvas.nCol; C++){
            Plc.x = -janela.wJ/2 + Dx/2  + C*Dx;
            
            Vetor dr = normalizar(Plc - olho_do_pintor);
            // como a projeção é perspectiva, o ray parte do olho do pintor
            Ray ray(olho_do_pintor, dr); 

            // =========== INTERSEÇÕES ===========

            // parâmetros de distância do ponto de interseção P(t)
            float ti_esf01, ti_chao, ti_fundo;

            // interseções nos objetos
            bool hit_esf01 = IntersecaoRayEsfera(esfera01, ray, ti_esf01);
            bool hit_chao = IntersecaoRayPlano(plano_chao, ray, ti_chao);
            bool hit_fundo = IntersecaoRayPlano(plano_fundo, ray, ti_fundo);

            // encontrar a interseção mais próxima
            float ti_min = -1.0f; // ti inicialmente inválido
            int objeto_mais_proximo = -1; // -1: nenhum, 0: esfera, 1: chão, 2: fundo
            
            // verificando em qual objeto o raio bateu primeiro
            if (hit_esf01 && ti_esf01 > 0 && (ti_min < 0 || ti_esf01 < ti_min)) {
                // se bateu na esfera com um ti válido e ti_esfera é o menor ti positivo, então
                ti_min = ti_esf01;
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

            /* caso o ID do objeto atingido seja válido, calcular
               ponto de interseção P(t) */
            if (objeto_mais_proximo != -1) {
                // P(t) = P_0 + t * dr
                Ponto Pt (  olho_do_pintor.x + ti_min * ray.dr.x, 
                            olho_do_pintor.y + ti_min * ray.dr.y, 
                            olho_do_pintor.z + ti_min * ray.dr.z  );
            
                // propriedades do objeto
                Vetor normal;
                Cor Kdif, Kesp, Kamb;
                Propriedades prop;
                int m;
                
                switch (objeto_mais_proximo) {
                case 0: // ESFERA01
                    normal = normalizar(Pt - esfera01.cEsfera);
                    Kdif = Kdif_esf01; Kesp = Kesp_esf01; Kamb = Kamb_esf01;
                    prop = Propriedades(Kdif, Kesp, Kamb);
                    m = m_esf01;
                    break;
                case 1: // CHÃO
                    normal = n_chao;
                    Kdif = Kdif_chao; Kesp = Kesp_chao; Kamb = Kamb_chao;
                    prop = Propriedades(Kdif, Kesp, Kamb);
                    m = m_chao;
                    break;
                case 2: // FUNDO
                    normal = n_fundo;
                    Kdif = Kdif_fundo; Kesp = Kesp_fundo; Kamb = Kamb_fundo;
                    prop = Propriedades(Kdif, Kesp, Kamb);
                    m = m_fundo;
                    break;
                default:
                    break;
                }

                // =========== SOMBRA ===========
                Vetor Ls = iluminacao.ponto_da_fonte - Pt;
                float distancia_da_Luz = comprimento(Ls);
                Ls = normalizar(Ls);

                Ponto Pt_sombra = normal * EPS + Pt;
                Ray shadowRay(Pt_sombra, Ls);

                bool sombra = false;
                float t;
                
                //  VERIFICANDO ONDE TEM SOBRA
                if (IntersecaoRayEsfera(esfera01, shadowRay, t) && t > 0 && t < distancia_da_Luz)
                    sombra = true;

                if (!sombra && IntersecaoRayPlano(plano_chao, shadowRay, t) && t > 0 && t < distancia_da_Luz)
                    sombra = true;

                if (!sombra && IntersecaoRayPlano(plano_fundo, shadowRay, t) && t > 0 && t < distancia_da_Luz)
                    sombra = true;

                IluminacaoCena iluminacao_com_sombra = iluminacao;
                if (sombra) {
                    iluminacao_com_sombra.intensidade_da_fonte = Cor(0,0,0);
                }

                canvasArray[L][C] = CorResultante(Pt, normal, ray, prop, iluminacao_com_sombra,
                m, objeto_mais_proximo);
            }
        }
    }

    // gerar aquivo ppm
    ofstream arquivo("output/tarefa03/esfera&planos_v2.ppm");
    arquivo << "P3\n" << canvas.nCol << " " << canvas.nLin << "\n255\n";
    for (size_t L = 0; L < canvas.nLin; L++)
    {
        for (size_t C = 0; C < canvas.nCol; C++)
        {
            arquivo << canvasArray[L][C].r * 255 << " " << canvasArray[L][C].g * 255 << " " << canvasArray[L][C].b * 255 << " ";
        }
        arquivo << "\n";
    }
    arquivo.close();
    cout << "Imagem gerada." << endl;

    return 0;
}