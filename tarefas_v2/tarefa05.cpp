/*  Compilação: 
 g++ -std=c++17 -Iinclude src/auxiliares/*.cpp src/objetos/*.cpp tarefas/tarefa05.cpp 
 -o output/tarefa05/tarefa05.exe
    Execução:
.\output\tarefa05\tarefa05.exe
*/
// auxiliares
#include "auxiliares/Vetores.h"
#include "auxiliares/Pontos.h"
#include "auxiliares/Janela.h"
#include "auxiliares/Ray.h"
#include "auxiliares/Canvas.h"
#include "auxiliares/Iluminacao.h"
#include "auxiliares/Textura.h"
// objetos
#include "objetos/Esfera.h"
#include "objetos/Plano.h"
#include "objetos/Cilindro.h"
#include "objetos/Cone.h"
#include "objetos/Cubo.h"
#include "objetos/ListMesh.h"
#include "objetos/Triangulo.h"
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
    Ponto PF(-100.0f, 140.0f, -20.0f);

    IluminacaoCena iluminacao(IF, IA, PF);

    // =========== TEXTURAS ===========
    Textura texturaChao;
    Textura texturaParede;
    
    // Carregar texturas
    if (!texturaChao.carregar("assets/pisoTextura.png")) {
        std::cout << "erro na textura do chão" << std::endl;
    }

    // =========== PARÂMETRO DOS OBJETOS ===========
        //  ESFERA01
        float rEsfera01 = 5.0f;
        Ponto cEsfera01(0.0f, 95.0f, -200.0f);
        Cor esfColor01(normalizaRGB(255, 0, 0));
            Cor Kdif_esf01, Kesp_esf01, Kamb_esf01 = Cor(0.854, 0.647, 0.125);       
        int m_esf01 = 10;   
        Propriedades propEsf01(Kdif_esf01, Kesp_esf01, Kamb_esf01);

        // PLANO_CHAO
        Ponto P_chao(0.0f, -150.0f, 0.0f);
        Vetor n_chao (0, 1, 0);
            Cor Kdif_chao(0.2f, 0.7f, 0.2f);
            Cor Kesp_chao(0.0f, 0.0f, 0.0f);
            Cor Kamb_chao(0.2f, 0.7f, 0.2f);
        int m_chao = 1;
        Propriedades propPlano_chao(Kdif_chao, Kesp_chao, Kamb_chao);

        // PLANO_LD
        Ponto P_LD(200.0f, -150.0f, 0.0f);
        Vetor n_LD (-1, 0, 0);
            Cor Kdif_LD, Kesp_LD, Kamb_LD = Cor(0.686, 0.933, 0.933);
        int m_LD = 1;
        Propriedades propPlano_LD(Kdif_LD, Kesp_LD, Kamb_LD);

        // PLANO_FRONTAL
        Ponto P_FRONTAL(200.0f, -150.0f, -400.0f);
        Vetor n_FRONTAL(0, 0, 1);
            Cor Kdif_FRONTAL, Kesp_FRONTAL, Kamb_FRONTAL = Cor(0.686, 0.933, 0.933);
        int m_FRONTAL = 1;
        Propriedades propPlano_FRONTAL(Kdif_FRONTAL, Kesp_FRONTAL, Kamb_FRONTAL);

        // PLANO_LE
        Ponto P_LE(-200.0f, -150.0f, 0.0f);
        Vetor n_LE (1, 0, 0);
            Cor Kdif_LE, Kesp_LE, Kamb_LE = Cor(0.686, 0.933, 0.933);
        int m_LE = 1;
        Propriedades propPlano_LE(Kdif_LE, Kesp_LE, Kamb_LE);

        // PLANO_TETO
        Ponto P_TETO(0.0f, 150.0f, 0.0f);
        Vetor n_TETO (0, -1, 0);
            Cor Kdif_TETO, Kesp_TETO, Kamb_TETO = Cor(0.933, 0.933, 0.933);
        int m_TETO = 1;
        Propriedades propPlano_TETO(Kdif_TETO, Kesp_TETO, Kamb_TETO);

        // CILINDRO01
        Ponto cbCilindro01(0.0f, -150.0f, -200.0f);
        float rbCilindro01 = 5.0f;
        float alturaCilindro01 = 90.0f;
        Vetor dcil01(0, 1, 0);
            Cor Kdif_cil01, Kesp_cil01, Kamb_cil01 = Cor(0.824, 0.706, 0.549);
        int m_cil01 = 10;
        Propriedades propCil01(Kdif_cil01, Kesp_cil01, Kamb_cil01);

        // CONE01
        Ponto cbCone01(0.0f, -60.0f, -200.0f);
        float rbCone01 = 90.0f;
        float alturaCone01 = 150.0f;
        Vetor dco01(0, 1, 0);
            Cor Kdif_cone01, Kesp_cone01, Kamb_cone01 = Cor(0., 1., 0.498);
        int m_cone01 = 10;
        Propriedades propCone01(Kdif_cone01, Kesp_cone01, Kamb_cone01);

        // CUBO01
        Ponto cbCubo01(0.0f, -150.0f, -165.0f);
        float arestaCubo01 = 40.0f;
            Cor Kdif_cubo01, Kesp_cubo01, Kamb_cubo01 = Cor(0, 1, 0.498);
        int m_cubo01 = 10;                          
        Propriedades propCubo01(Kdif_cubo01, Kesp_cubo01, Kamb_cubo01);

    // =========== OBJETOS ===========
    // ESFERA01
    Esfera esfera01(rEsfera01, cEsfera01, esfColor01,
    propEsf01, m_esf01);
    // PLANO_CHAO
    Plano plano_chao(P_chao, n_chao, propPlano_chao, m_chao);
    plano_chao.setTextura(&texturaChao);
    // PLANO_LD
    Plano plano_LD(P_LD, n_LD, propPlano_LD, m_LD);
    // PLANO_FRONTAL
    Plano plano_FRONTAL(P_FRONTAL, n_FRONTAL, propPlano_FRONTAL, m_FRONTAL);
    // PLANO_LE
    Plano plano_LE(P_LE, n_LE, propPlano_LE, m_LE);
    // PLANO_TETO
    Plano plano_TETO(P_TETO, n_TETO, propPlano_TETO, m_TETO);
    // CILINDRO01
    Cilindro cilindro01(cbCilindro01, rbCilindro01, alturaCilindro01, dcil01, propCil01, m_cil01);
    // CONE01
    Cone cone01(cbCone01, rbCone01, alturaCone01, dco01, propCone01, m_cone01);
    // CUBO01
    Cubo cubo01(cbCubo01, arestaCubo01, propCubo01, m_cubo01);

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
            float ti_esf01, ti_chao, ti_LD, ti_FRONTAL, ti_LE,
            ti_TETO, ti_cil01, ti_cone01, ti_cubo01;

            // interseções nos objetos
            bool hit_esf01 = IntersecaoRayEsfera(esfera01, ray, ti_esf01);
            bool hit_chao = IntersecaoRayPlano(plano_chao, ray, ti_chao);
            bool hit_LD = IntersecaoRayPlano(plano_LD, ray, ti_LD);
            bool hit_FRONTAL = IntersecaoRayPlano(plano_FRONTAL, ray, ti_FRONTAL);
            bool hit_LE = IntersecaoRayPlano(plano_LE, ray, ti_LE);
            bool hit_TETO = IntersecaoRayPlano(plano_TETO, ray, ti_TETO);
            bool hit_cil01 = IntersecaoRayCilindro(cilindro01, ray, ti_cil01);
            bool hit_cone01 = IntersecaoRayCone(cone01, ray, ti_cone01);
            bool hit_cubo01 = IntersecaoRayCubo(cubo01, ray, ti_cubo01);

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
            if (hit_LD && ti_LD > 0 && (ti_min < 0 || ti_LD < ti_min)) {
                ti_min = ti_LD;
                objeto_mais_proximo = 2;
            }
            if (hit_FRONTAL && ti_FRONTAL > 0 && (ti_min < 0 || ti_FRONTAL < ti_min)) {
                ti_min = ti_FRONTAL;
                objeto_mais_proximo = 3;
            }
            if (hit_LE && ti_LE > 0 && (ti_min < 0 || ti_LE < ti_min)) {
                ti_min = ti_LE;
                objeto_mais_proximo = 4;
            }
            if (hit_TETO && ti_TETO > 0 && (ti_min < 0 || ti_TETO < ti_min)) {
                ti_min = ti_TETO;
                objeto_mais_proximo = 5;
            }
            if (hit_cil01 && ti_cil01 > 0 && (ti_min < 0 || ti_cil01 < ti_min)) {
                ti_min = ti_cil01;
                objeto_mais_proximo = 6;
            }
            if (hit_cone01 && ti_cone01 > 0 && (ti_min < 0 || ti_cone01 < ti_min)) {
                ti_min = ti_cone01;
                objeto_mais_proximo = 7;
            }
            if (hit_cubo01 && ti_cubo01 > 0 && ti_cubo01 < ti_min) {
                ti_min = ti_cubo01;
                objeto_mais_proximo = 8;
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
                {
                    normal = n_chao;
                    Cor corTextura = plano_chao.getCorTextura(Pt);

                    Kdif = corTextura; Kesp = Cor(0.1f, 0.1f, 0.1f); Kamb = corTextura * 0.3f;
                    prop = Propriedades(Kdif, Kesp, Kamb);
                    m = m_chao;
                    break;
                }
                case 2: // LD
                    normal = n_LD;
                    Kdif = Kdif_LD; Kesp = Kesp_LD; Kamb = Kamb_LD;
                    prop = Propriedades(Kdif, Kesp, Kamb);
                    m = m_LD;
                    break;
                case 3: // FRONTAL
                    normal = n_FRONTAL;
                    Kdif = Kdif_FRONTAL; Kesp = Kesp_FRONTAL; Kamb = Kamb_FRONTAL;
                    prop = Propriedades(Kdif, Kesp, Kamb);
                    m = m_FRONTAL;
                    break;
                case 4: // LE
                    normal = n_LE;
                    Kdif = Kdif_LE; Kesp = Kesp_LE; Kamb = Kamb_LE;
                    prop = Propriedades(Kdif, Kesp, Kamb);
                    m = m_LE;
                    break;
                case 5: // TETO
                    normal = n_TETO;
                    Kdif = Kdif_TETO; Kesp = Kesp_TETO; Kamb = Kamb_TETO;
                    prop = Propriedades(Kdif, Kesp, Kamb);
                    m = m_TETO;
                    break;
                case 6: // CILINDRO01
                    normal = normalCilindro(cilindro01, Pt);
                    Kdif = Kdif_cil01; Kesp = Kesp_cil01; Kamb = Kamb_cil01;
                    prop = Propriedades(Kdif, Kesp, Kamb);
                    m = m_cil01;
                    break;
                case 7: // CONE01  
                    normal = normalCone(cone01, Pt);
                    Kdif = Kdif_cone01; Kesp = Kesp_cone01; Kamb = Kamb_cone01;
                    prop = Propriedades(Kdif, Kesp, Kamb);
                    m = m_cone01;
                    break;
                case 8: // CUBO01
                    // Para o cubo, a normal é a do triângulo atingido
                    // Precisamos verificar qual triângulo foi atingido
                    normal = normalCubo(cubo01, Pt);
                    Kdif = Kdif_cubo01; Kesp = Kesp_cubo01; Kamb = Kamb_cubo01;
                    prop = Propriedades(Kdif, Kesp, Kamb);
                    m = m_cubo01;
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

                if (!sombra && IntersecaoRayPlano(plano_LD, shadowRay, t) && t > 0 && t < distancia_da_Luz)
                    sombra = true;
                
                if (!sombra && IntersecaoRayPlano(plano_FRONTAL, shadowRay, t) && t > 0 && t < distancia_da_Luz)
                    sombra = true;
                
                if (!sombra && IntersecaoRayPlano(plano_LE, shadowRay, t) && t > 0 && t < distancia_da_Luz)
                    sombra = true;

                if (!sombra && IntersecaoRayPlano(plano_TETO, shadowRay, t) && t > 0 && t < distancia_da_Luz)
                    sombra = true;
                
                if (!sombra && IntersecaoRayCilindro(cilindro01, shadowRay, t) && t > 0 && t < distancia_da_Luz)
                    sombra = true;

                if (!sombra && IntersecaoRayCone(cone01, shadowRay, t) && t > 0 && t < distancia_da_Luz)
                    sombra = true;
                
                if (!sombra && IntersecaoRayCubo(cubo01, shadowRay, t) && t > 0 && t < distancia_da_Luz)
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
    ofstream arquivo("output/tarefa05/tarefa05_v2.ppm");
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