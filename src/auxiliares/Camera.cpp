#include "auxiliares/Camera.h"
#include <cmath>


Camera::Camera() 
    : eye(0, 0, 0), 
    AtPoint(0, 0, -1), 
    UpPoint(0, 1, 0),
    distanciaFocal(30.0f),
    janela(60.0f, 60.0f, 30.0f),
    xmin(-30.0f), xmax(30.0f),
    ymin(-30.0f), ymax(30.0f) {
    atualizarSistemaCoordenadas();
}

Camera::Camera(const Ponto& eye, const Ponto& AtPoint, const Vetor& up, 
               float distanciaFocal, const Janela& janela)
    : eye(eye), AtPoint(AtPoint), UpPoint(up), 
      distanciaFocal(distanciaFocal), janela(janela),
      xmin(-janela.wJ/2.0f),      
      xmax(janela.wJ/2.0f),       
      ymin(-janela.hJ/2.0f),      
      ymax(janela.hJ/2.0f)  {
    atualizarSistemaCoordenadas();
}

void Camera::lookAt(const Ponto& novoOlho, const Ponto& novoAlvo, const Vetor& up) {
    eye = novoOlho;
    AtPoint = novoAlvo;
    UpPoint = up;
    atualizarSistemaCoordenadas();
}

void Camera::setJanela(const Janela& jan) {
    janela = jan;
}

void Camera::setDistanciaFocal(float dist) {
    distanciaFocal = dist;
}

void Camera::atualizarSistemaCoordenadas() {
    Vetor k = normalizar(eye - AtPoint); 
    direita = normalizar(produto_vetorial(UpPoint, k));
    acima = produto_vetorial(k, direita);
    frente = k * -1.0f; 
}

Matriz4x4 Camera::getMatrizCameraMundo() const {
    Matriz4x4 m;
    
    m.m[0][0] = direita.x; m.m[1][0] = direita.y; m.m[2][0] = direita.z;
    m.m[0][1] = acima.x;   m.m[1][1] = acima.y;   m.m[2][1] = acima.z;
    m.m[0][2] = -frente.x; m.m[1][2] = -frente.y; m.m[2][2] = -frente.z;
    m.m[0][3] = eye.x;     m.m[1][3] = eye.y;     m.m[2][3] = eye.z;
    
    m.m[3][0] = 0; m.m[3][1] = 0; m.m[3][2] = 0; m.m[3][3] = 1;

    return m;
}

Ray Camera::gerarRaioParaPixel(int linha, int coluna, const Canvas& canvas) const {
    float u = (coluna + 0.5f) / canvas.nCol;       
    float v = 1.0f - (linha + 0.5f) / canvas.nLin; 
    
    return gerarRaio(u, v);
}

Ray Camera::gerarRaio(float u, float v) const {
    float x_cam = (u - 0.5f) * janela.wJ;
    float y_cam = (v - 0.5f) * janela.hJ;
    float z_cam = -distanciaFocal;

    Matriz4x4 camToWorld = getMatrizCameraMundo();

    if (projecaoAtual == PERSPECTIVA) {
        Ponto pontoMundo = camToWorld * Ponto(x_cam, y_cam, z_cam);
        return Ray(eye, normalizar(pontoMundo - eye));
    } 
    else if (projecaoAtual == ORTOGRAFICA) {
        Ponto origemRaioMundo = camToWorld * Ponto(x_cam, y_cam, 0); 
        return Ray(origemRaioMundo, frente);
    } 
    else { 
        float alpha = 45.0f * M_PI / 180.0f; 
        
        //  (f = 1.0 para Cavalier, f = 0.5 para Cabinet)
        float f = 0.5f; 

        Vetor direcaoLocal(f * cos(alpha), f * sin(alpha), -1.0f);
        
        Ponto origemRaioMundo = camToWorld * Ponto(x_cam, y_cam, 0);
        Vetor direcaoMundo = normalizar(camToWorld * direcaoLocal); 

        return Ray(origemRaioMundo, direcaoMundo);
    }
}

Ponto Camera::calcularPontoJanela(float u, float v) const {
    Ponto centroJanela = (frente * distanciaFocal) + eye;
    
    float deslocX = u * janela.wJ;
    float deslocY = v * janela.hJ;
    
    Ponto pontoJanela = (direita * deslocX) + (acima * deslocY) + centroJanela;
    
    return pontoJanela;
}

void Camera::setProjecao(TipoProjecao p, const Canvas& canvas) {
    projecaoAtual = p;

    if (p == ORTOGRAFICA || p == OBLIQUA) {
        janela.wJ = (float)canvas.nCol;
        janela.hJ = (float)canvas.nLin;
    } else {
        janela.wJ = 60.0f;
        janela.hJ = 60.0f;
    }
    
    xmin = -janela.wJ / 2.0f;
    xmax =  janela.wJ / 2.0f;
    ymin = -janela.hJ / 2.0f;
    ymax =  janela.hJ / 2.0f;
}

void Camera::setCampoDeVisao(float novo_xmin, float novo_xmax, float novo_ymin, float novo_ymax) {
    xmin = novo_xmin;
    xmax = novo_xmax;
    ymin = novo_ymin;
    ymax = novo_ymax;
    
    janela.wJ = xmax - xmin;
    janela.hJ = ymax - ymin;
}

// ============== MOVIMENTAÇÃO DA CÂMERA ==============

void Camera::mover(const Vetor& deslocamento) {
    eye = eye + deslocamento;
    AtPoint = AtPoint + deslocamento;
    atualizarSistemaCoordenadas();
}

void Camera::moverFrente(float distancia) { mover(frente * distancia); }
void Camera::moverDireita(float distancia) { mover(direita * distancia); }
void Camera::moverCima(float distancia) { mover(acima * distancia); }

void Camera::rotacionarEmY(float anguloGraus) {
    Vetor eixoY(0, 1, 0);
    Matriz4x4 rot = Transformacao::rotacaoEixoArbitrarioOrigem(eixoY, anguloGraus);
    
    Vetor novaFrente = rot * frente;
    AtPoint = eye + (novaFrente * distanciaFocal);
    atualizarSistemaCoordenadas();
}

void Camera::rotacionarEmX(float anguloGraus) {
    Matriz4x4 rot = Transformacao::rotacaoEixoArbitrarioOrigem(direita, anguloGraus);
    
    Vetor novaFrente = rot * frente;
    
    if (std::abs(novaFrente.y) < 0.98f) { 
        AtPoint = eye + (novaFrente * distanciaFocal);
        atualizarSistemaCoordenadas();
    }
}

void Camera::rotacionarEmZ(float anguloGraus) {
    Matriz4x4 rot = Transformacao::rotacaoEixoArbitrarioOrigem(frente, anguloGraus);
    
    UpPoint = rot * acima; 
    atualizarSistemaCoordenadas();
}

void Camera::zoom(float fator) {
    distanciaFocal *= fator;
    janela.wJ *= fator;
    janela.hJ *= fator;
}