// g++ -std=c++17 -Iinclude src/auxiliares/*.cpp src/objetos/*.cpp tarefas_v2/tarefa05.cpp -o output/tarefa05/tarefa05.exe
// .\output\tarefa05\tarefa05.exe
#include "auxiliares/Camera.h"
#include "auxiliares/Canvas.h"
#include "auxiliares/Iluminacao.h"
#include "auxiliares/Textura.h"
#include "auxiliares/Utils.h"  
#include "objetos/Objeto.h"
#include "objetos/Esfera.h"
#include "objetos/Plano.h"
#include "objetos/Cilindro.h"
#include "objetos/Cone.h"
#include "objetos/Cubo.h"

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>

using namespace std;

int main() {
    const float EPS = 1e-3f;
    
    // =========== JANELA ===========
    Janela janela(60.0f, 60.0f, 30.0f);

    // =========== CÂMERA ===========
    Camera camera;
    Ponto eye(0.0f, 0.0f, 0.0f);
    Ponto atpoint(0.0f, 0.0f, -janela.dJ);
    Vetor uppoint(0.0f, 1.0f, 0.0f);
    
    camera.lookAt(eye, atpoint, uppoint);
    camera.setJanela(janela);
    camera.setDistanciaFocal(30.0f);

    // =========== CANVAS ===========
    Canvas canvas(500, 500);

    // =========== ILUMINAÇÃO ===========
    Cor IF(0.7f, 0.7f, 0.7f);
    Cor IA(0.3f, 0.3f, 0.3f);
    Ponto PF(-100.0f, 140.0f, -20.0f);
    IluminacaoCena iluminacao(IF, IA, PF);

    // =========== TEXTURAS ===========
    Textura texturaChao;
    if (!texturaChao.carregar("assets/pisoTextura.png")) {
        cout << "Erro ao carregar textura do chão" << endl;
    }

    // =========== CRIAR OBJETOS ===========
    vector<unique_ptr<Objeto>> objetos;
    
    // ESFERA01
    objetos.push_back(make_unique<Esfera>(
        5.0f,
        Ponto(0.0f, 95.0f, -200.0f),
        normalizaRGB(255, 0, 0),
        Propriedades(
            Cor(0.854f, 0.647f, 0.125f),
            Cor(0.854f, 0.647f, 0.125f),
            Cor(0.854f, 0.647f, 0.125f)
        ),
        10
    ));
    
    // PLANO_CHAO com textura
    auto planoChao = make_unique<Plano>(
        Ponto(0.0f, -150.0f, 0.0f),
        Vetor(0, 1, 0),
        Propriedades(
            Cor(0.2f, 0.7f, 0.2f),
            Cor(0.0f, 0.0f, 0.0f),
            Cor(0.2f, 0.7f, 0.2f)
        ),
        1
    );
    planoChao->setTextura(&texturaChao);
    objetos.push_back(std::move(planoChao));
    
    // PLANO_LD
    objetos.push_back(make_unique<Plano>(
        Ponto(200.0f, -150.0f, 0.0f),
        Vetor(-1, 0, 0),
        Propriedades(
            Cor(0.686f, 0.933f, 0.933f),
            Cor(0.686f, 0.933f, 0.933f),
            Cor(0.686f, 0.933f, 0.933f)
        ),
        1
    ));
    
    // PLANO_FRONTAL
    objetos.push_back(make_unique<Plano>(
        Ponto(200.0f, -150.0f, -400.0f),
        Vetor(0, 0, 1),
        Propriedades(
            Cor(0.686f, 0.933f, 0.933f),
            Cor(0.686f, 0.933f, 0.933f),
            Cor(0.686f, 0.933f, 0.933f)
        ),
        1
    ));
    
    // PLANO_LE
    objetos.push_back(make_unique<Plano>(
        Ponto(-200.0f, -150.0f, 0.0f),
        Vetor(1, 0, 0),
        Propriedades(
            Cor(0.686f, 0.933f, 0.933f),
            Cor(0.686f, 0.933f, 0.933f),
            Cor(0.686f, 0.933f, 0.933f)
        ),
        1
    ));
    
    // PLANO_TETO
    objetos.push_back(make_unique<Plano>(
        Ponto(0.0f, 150.0f, 0.0f),
        Vetor(0, -1, 0),
        Propriedades(
            Cor(0.933f, 0.933f, 0.933f),
            Cor(0.933f, 0.933f, 0.933f),
            Cor(0.933f, 0.933f, 0.933f)
        ),
        1
    ));
    
    // CILINDRO01
    objetos.push_back(make_unique<Cilindro>(
        Ponto(0.0f, -150.0f, -200.0f),
        5.0f,
        90.0f,
        Vetor(0, 1, 0),
        Propriedades(
            Cor(0.824f, 0.706f, 0.549f),
            Cor(0.824f, 0.706f, 0.549f),
            Cor(0.824f, 0.706f, 0.549f)
        ),
        10
    ));
    
    // CONE01
    objetos.push_back(make_unique<Cone>(
        Ponto(0.0f, -60.0f, -200.0f),
        90.0f,
        150.0f,
        Vetor(0, 1, 0),
        Propriedades(
            Cor(0.0f, 1.0f, 0.498f),
            Cor(0.0f, 1.0f, 0.498f),
            Cor(0.0f, 1.0f, 0.498f)
        ),
        10
    ));
    
    // CUBO01
    objetos.push_back(make_unique<Cubo>(
        Ponto(0.0f, -150.0f, -165.0f),
        40.0f,
        Propriedades(
            Cor(0.0f, 1.0f, 0.498f),
            Cor(0.0f, 1.0f, 0.498f),
            Cor(0.0f, 1.0f, 0.498f)
        ),
        10
    ));
    
    // =========== RENDERIZAÇÃO ===========
    vector<vector<Cor>> canvasArray(canvas.nLin, vector<Cor>(canvas.nCol, canvas.bgColor));
    
    Utils::listarObjetos(objetos);
    
    // Loop principal de renderização
    for (size_t L = 0; L < canvas.nLin; L++) {
        for (size_t C = 0; C < canvas.nCol; C++) {
            Ray ray = camera.gerarRaioParaPixel(L, C, canvas);
            
            IntersecaoResultado intersecao;
            if (Utils::encontrarIntersecaoMaisProxima(ray, objetos, intersecao, EPS)) {
                // Calcular cor final com tratamento automático de textura
                Cor corFinal = Utils::calcularCorFinal(intersecao, ray, iluminacao, objetos, EPS);
                canvasArray[L][C] = corFinal;
            }
        }
    }
    
    // =========== SALVAR IMAGEM ===========
    ofstream arquivo("output/tarefa05/tarefa05.ppm");
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