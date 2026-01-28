// g++ -std=c++17 -Iinclude src/auxiliares/*.cpp src/objetos/*.cpp TrabalhoFinal/TrabalhoFinal.cpp -o TrabalhoFinal/output/TrabalhoFinal.exe
// .\TrabalhoFinal/output/TrabalhoFinal.exe

#include "auxiliares/Camera.h"
#include "auxiliares/Canvas.h"
#include "auxiliares/Iluminacao.h"
#include "auxiliares/Textura.h"
#include "auxiliares/Renderizacao.h"  
#include "objetos/Objeto.h"
#include "objetos/Esfera.h"
#include "objetos/Plano.h"
#include "objetos/Cilindro.h"
#include "objetos/Cone.h"
#include "objetos/Cubo.h"
#include "objetos/Arvore.h"
#include "objetos/Paralelepipedo.h"
#include "objetos/Cano.h"
#include "objetos/Cogumelo.h"
#include "objetos/Bandeira.h"
#include "objetos/Casco.h"
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>

using namespace std;

int main() {
    const float EPS = 1e-3f;
    
    const float CENARIO_TAMANHO = 500.0f; 
    const float CENARIO_CENTRO_X = CENARIO_TAMANHO / 2.0f;
    const float CENARIO_CENTRO_Y = CENARIO_TAMANHO / 2.0f;
    const float CENARIO_CENTRO_Z = CENARIO_TAMANHO / 2.0f;
    
    // =========== JANELA ===========
    Janela janela(60.0f, 60.0f, 30.0f);

    // =========== CÂMERA ===========
    Camera camera;
    
    Ponto eye(250.0f, 250.0f, 0.0f);
    Ponto atpoint(CENARIO_CENTRO_X, CENARIO_CENTRO_Y * 0.3f, CENARIO_CENTRO_Z); 
    Vetor uppoint(0.0f, 1.0f, 0.0f);
    
    camera.lookAt(eye, atpoint, uppoint);
    camera.setJanela(janela);
    camera.setDistanciaFocal(30.0f);

   
    // =========== CANVAS ===========
    Canvas canvas(500, 500);

    // =========== ILUMINAÇÃO ===========
    Cor IF(0.6f, 0.6f, 0.6f);      
    Cor IA(0.3f, 0.3f, 0.3f);
    Ponto PF(CENARIO_CENTRO_X, CENARIO_TAMANHO * 0.8f, CENARIO_CENTRO_Z);  
    IluminacaoCena iluminacao(IF, IA, PF);

    // =========== TEXTURAS ===========
    Textura texturaChao;
    if (!texturaChao.carregar("assets/marTextura01.jpg")) {
        cout << "Erro ao carregar textura" << endl;
    }

    Textura gramaTextura01;
    if (!gramaTextura01.carregar("assets/gramaTextura01.jpg")) {
        cout << "Erro ao carregar textura" << endl;
    }

    Textura tijoloMarioTextura01;
    if (!tijoloMarioTextura01.carregar("assets/tijoloMarioTextura01.png")) {
        cout << "Erro ao carregar textura" << endl;
    }

    Textura blocoMarioTextura02;
    if (!blocoMarioTextura02.carregar("assets/blocoMarioTextura02.png")) {
        cout << "Erro ao carregar textura" << endl;
    }
    // =========== CRIAR OBJETOS ===========
    vector<Objeto*> objetos;
    vector<ObjetoComplexo*> objetosComplexos;


    // ------------------ COGUMELO ------------------
    Cogumelo cogumelo01(Ponto(145.0f, 200.0f, 330.0f), 10.0f, 13.0f, 15.0f);
    
    objetosComplexos.push_back(&cogumelo01);
    objetos.push_back(&cogumelo01);


    // ------------------ ÁRVORES ------------------
    Arvore arvore01(Ponto(455.0f, 70.0f, 350.0f), 50.0f, 5.0f, 25.0f, 15);
    objetosComplexos.push_back(&arvore01);
    objetos.push_back(&arvore01);

    Arvore arvore02(Ponto(455.0f, 70.0f, 300.0f), 50.0f, 5.0f, 25.0f, 15);
    objetosComplexos.push_back(&arvore02);
    objetos.push_back(&arvore02);

    Arvore arvore03(Ponto(455.0f, 70.0f, 250.0f), 50.0f, 5.0f, 25.0f, 15);
    objetosComplexos.push_back(&arvore03);
    objetos.push_back(&arvore03);

    Arvore arvore04(Ponto(455.0f, 70.0f, 115.0f), 50.0f, 5.0f, 25.0f, 15);
    objetosComplexos.push_back(&arvore04);
    objetos.push_back(&arvore04);

    Arvore arvore05(Ponto(455.0f, 70.0f, 65.0f), 50.0f, 5.0f, 25.0f, 15);
    objetosComplexos.push_back(&arvore05);
    objetos.push_back(&arvore05);

    Arvore arvore06(Ponto(455.0f, 70.0f, 15.0f), 50.0f, 5.0f, 25.0f, 15);
    objetosComplexos.push_back(&arvore06);
    objetos.push_back(&arvore06);

    Arvore arvore07(Ponto(45.0f, 70.0f, 350.0f), 50.0f, 5.0f, 25.0f, 15);
    objetosComplexos.push_back(&arvore07);
    objetos.push_back(&arvore07);

    Arvore arvore08(Ponto(45.0f, 70.0f, 300.0f), 50.0f, 5.0f, 25.0f, 15);
    objetosComplexos.push_back(&arvore08);
    objetos.push_back(&arvore08);

    Arvore arvore09(Ponto(45.0f, 70.0f, 250.0f), 50.0f, 5.0f, 25.0f, 15);
    objetosComplexos.push_back(&arvore09);
    objetos.push_back(&arvore09);


    // ------------------ BANDEIRA ------------------
    Bandeira bandeira(Ponto(240.0f, 70.0f, 50.0f), 120.0f, 15);
    objetosComplexos.push_back(&bandeira);
    objetos.push_back(&bandeira);


    // ------------------ CASCO ------------------
    Casco casco(Ponto(110.0f, 65.0f, 90.0f), 12.0f, 10);

    objetosComplexos.push_back(&casco);
    objetos.push_back(&casco);


    // ------------------ CANOS ------------------
    Cano cano01(Ponto(245.0f, 0.0f, 300.0f), 25.0f, 90.0f, 35.0f, 20.0f, 
        Vetor(0.0f, 1.0f, 0.0f), Propriedades(Cor(0.0f, 0.8f, 0.4f), Cor(0.4f, 0.4f, 0.4f), Cor(0.0f, 0.2f, 0.1f)
        ), 40.0f);
    
    objetosComplexos.push_back(&cano01);    
    objetos.push_back(&cano01);    

    Cano cano02(Ponto(390.0f, 0.0f, 175.0f), 25.0f, 90.0f, 35.0f, 20.0f, 
        Vetor(0.0f, 1.0f, 0.0f), Propriedades(Cor(0.0f, 0.8f, 0.4f), Cor(0.4f, 0.4f, 0.4f), Cor(0.0f, 0.2f, 0.1f)
        ), 40.0f);
    
    objetosComplexos.push_back(&cano02);    
    objetos.push_back(&cano02);    


    // ------------------ PLATAFORMAS ------------------
    Paralelepipedo plataforma01(Ponto(110.0f, 0.0f, 320.0f), 160.0f, 150.0f, 180.0f, 
            Propriedades(Cor(0.5f, 0.5f, 0.5f), Cor(0.02f, 0.02f, 0.02f), Cor(0.1f, 0.1f, 0.1f)
        ), 10);
    
    plataforma01.setTextura(&gramaTextura01);
    objetos.push_back(&plataforma01);

    Paralelepipedo plataforma02(Ponto(340.0f, 0.0f, 320.0f), 180.0f, 150.0f, 220.0f, 
            Propriedades(Cor(0.5f, 0.5f, 0.5f), Cor(0.02f, 0.02f, 0.02f), Cor(0.1f, 0.1f, 0.1f)
        ), 10);
    
    plataforma02.transladar(50.0f, 0.0f, 0.0f);
    plataforma02.setTextura(&gramaTextura01);
    objetos.push_back(&plataforma02);

    Paralelepipedo plataforma03(Ponto(340.0f, 0.0f, 40.0f), 270.0f, 150.0f, 180.0f, 
            Propriedades(Cor(0.5f, 0.5f, 0.5f), Cor(0.02f, 0.02f, 0.02f), Cor(0.1f, 0.1f, 0.1f)
        ), 10);
    
    plataforma03.setTextura(&gramaTextura01);
    objetos.push_back(&plataforma03);


    // ------------------ CAMINHOS ------------------
    Paralelepipedo caminho01(Ponto(110.0f, 50.0f, 185.0f), 25.0f, 25.0f, 80.0f, 
            Propriedades(Cor(0.8f, 0.8f, 0.8f), Cor(0.1f, 0.1f, 0.1f), Cor(0.2f, 0.2f, 0.2f)
        ), 5);
    
    caminho01.setEscalaTextura(0.4f);
    caminho01.setTextura(&tijoloMarioTextura01);
    objetos.push_back(&caminho01);

    Paralelepipedo caminho02(Ponto(110.0f, 50.0f, 90.0f), 25.0f, 25.0f, 50.0f, 
            Propriedades(Cor(0.8f, 0.8f, 0.8f), Cor(0.1f, 0.1f, 0.1f), Cor(0.2f, 0.2f, 0.2f)
        ), 5);
    
    caminho02.setEscalaTextura(0.4f);
    caminho02.setTextura(&tijoloMarioTextura01);
    objetos.push_back(&caminho02);

    Paralelepipedo caminho03(Ponto(180.0f, 50.0f, 75.0f), 25.0f, 25.0f, 50.0f, 
            Propriedades(Cor(0.8f, 0.8f, 0.8f), Cor(0.1f, 0.1f, 0.1f), Cor(0.2f, 0.2f, 0.2f)
        ), 5);
    
    caminho03.rotacionarY(90.0f);
    caminho03.setEscalaTextura(0.4f);
    caminho03.setTextura(&tijoloMarioTextura01);
    objetos.push_back(&caminho03);


    // ------------------ BLOCOS ------------------
    Cubo bloco01(Ponto(115.0f, 175.0f, 330.0f), 30.0f, 
        Propriedades(Cor(0.7f, 0.5f, 0.1f), Cor(0.9f, 0.8f, 0.4f), Cor(0.3f, 0.2f, 0.0f)
        ), 100);

    bloco01.setTextura(&blocoMarioTextura02);
    objetos.push_back(&bloco01);

    Cubo bloco02(Ponto(175.0f, 175.0f, 330.0f), 30.0f, 
        Propriedades(Cor(0.7f, 0.5f, 0.1f), Cor(0.9f, 0.8f, 0.4f), Cor(0.3f, 0.2f, 0.0f)
        ), 100);

    bloco02.setTextura(&blocoMarioTextura02);
    objetos.push_back(&bloco02);

    Cubo bloco03(Ponto(145.0f, 175.0f, 330.0f), 30.0f, 
        Propriedades(Cor(0.8f, 0.8f, 0.8f), Cor(0.1f, 0.1f, 0.1f), Cor(0.2f, 0.2f, 0.2f)
        ), 20);
    
    bloco03.setEscalaTextura(0.4);
    bloco03.setTextura(&tijoloMarioTextura01);
    objetos.push_back(&bloco03);


    // ------------------ PLANOS ------------------
    Plano planoChao (
        Ponto(CENARIO_CENTRO_X, 0.0f, CENARIO_CENTRO_Z),  
        Vetor(0, 1, 0), 
        Propriedades(Cor(0.6f, 0.6f, 0.8f), Cor(0.4f, 0.4f, 0.4f), Cor(0.2f, 0.2f, 0.3f)
        ),
        60
    );
    planoChao.setTextura(&texturaChao);
    objetos.push_back(&planoChao);
    
    iluminacao.setDirecional(Vetor(1, -1, 1), Cor(0.8, 0.8, 0.8));
    // =========== RENDERIZAÇÃO ===========
    vector<vector<Cor>> canvasArray(canvas.nLin, vector<Cor>(canvas.nCol, canvas.bgColor));
    
    Renderiza::listarObjetos(objetos);
    
    for (size_t L = 0; L < canvas.nLin; L++) {
        for (size_t C = 0; C < canvas.nCol; C++) {
            Ray ray = camera.gerarRaioParaPixel(L, C, canvas);
            
            IntersecaoResultado intersecao;
            if (Renderiza::encontrarIntersecaoMaisProxima(ray, objetos, intersecao, EPS)) {
                Cor corFinal = Renderiza::calcularCorFinal(intersecao, ray, iluminacao, objetos, EPS);
                canvasArray[L][C] = corFinal;
            }
        }
    }
    
    // =========== SALVAR IMAGEM ===========
    ofstream arquivo("TrabalhoFinal/output/TrabalhoFinal.ppm");
    arquivo << "P3\n" << canvas.nCol << " " << canvas.nLin << "\n255\n";
    for (size_t L = 0; L < canvas.nLin; L++) {
        for (size_t C = 0; C < canvas.nCol; C++) {
            arquivo << canvasArray[L][C].r * 255 << " "
                   << canvasArray[L][C].g * 255 << " "
                   << canvasArray[L][C].b * 255 << " ";
        }
        arquivo << "\n";
    }
    arquivo.close();
    cout << "Imagem gerada." << endl;
    
    return 0;
}