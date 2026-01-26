#ifndef CAMERA_H
#define CAMERA_H

#include "Pontos.h"
#include "Vetores.h"
#include "Ray.h"
#include "Janela.h"
#include "Canvas.h"
#include "Matrizes.h"

// definindo valor de pi
#define M_PI 3.14159265358979323846

enum TipoProjecao { PERSPECTIVA, ORTOGRAFICA, OBLIQUA };

// ============== DEFINIÇÃO DA CLASSE ==============

class Camera {
private:
    Ponto eye;
    Ponto AtPoint;
    Vetor UpPoint;
    
    // Sistema de coordenadas da câmera
    Vetor frente, direita, acima;
    
    // Configurações
    float distanciaFocal;
    Janela janela;
    
    float xmin, xmax, ymin, ymax;

    TipoProjecao projecaoAtual = PERSPECTIVA;
    float obliqueAlpha = 45.0f; 
    float obliqueTheta = 45.0f;

public:
    // Construtores
    Camera();
    Camera(const Ponto& eye, const Ponto& AtPoint, const Vetor& up, 
           float distanciaFocal, const Janela& janela);
    
    // ============== MÉTODOS ASSOCIADOS À CÂMERA ==============

    // configuração
    void lookAt(const Ponto& eye, const Ponto& AtPoint, const Vetor& up = Vetor(0, 1, 0));
    void setJanela(const Janela& jan);
    void setDistanciaFocal(float dist);
    
    // acesso
    Ponto getOlho() const { return eye; }
    Ponto getAlvo() const { return AtPoint; }
    Vetor getFrente() const { return frente; }
    Vetor getDireita() const { return direita; }
    Vetor getAcima() const { return acima; }
    Janela getJanela() const { return janela; }
    
    // gera raio para um pixel
    Ray gerarRaioParaPixel(int linha, int coluna, const Canvas& canvas) const;
    
    // gera raio para coordenadas UV [0,1]
    Ray gerarRaio(float u, float v) const;

    void setCampoDeVisao(float novo_xmin, float novo_xmax, float novo_ymin, float novo_ymax);
    
    void setProjecao(TipoProjecao p, const Canvas& canvas);
    // manipulação da câmera
    void atualizarSistemaCoordenadas();
    Matriz4x4 getMatrizCameraMundo() const;
    void mover(const Vetor& deslocamento);
    void moverFrente(float distancia);
    void moverDireita(float distancia);
    void moverCima(float distancia);
    void rotacionarEmY(float anguloGraus);
    void rotacionarEmX(float anguloGraus);
    void rotacionarEmZ(float anguloGraus);
    void zoom(float fator);

    
private:
    // calcula ponto na janela
    Ponto calcularPontoJanela(float u, float v) const;
 
};


#endif