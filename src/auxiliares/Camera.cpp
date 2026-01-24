#include "auxiliares/Camera.h"
#include <cmath>


Camera::Camera() 
    : eye(0, 0, 0), 
      AtPoint(0, 0, -1), 
      UpPoint(0, 1, 0),
      distanciaFocal(30.0f),
      janela(60.0f, 60.0f, 30.0f) {
    atualizarSistemaCoordenadas();
}

Camera::Camera(const Ponto& eye, const Ponto& AtPoint, const Vetor& up, 
               float distanciaFocal, const Janela& janela)
    : eye(eye), AtPoint(AtPoint), UpPoint(up), 
      distanciaFocal(distanciaFocal), janela(janela) {
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
    frente = normalizar(AtPoint - eye);
    
    direita = normalizar(produto_vetorial(frente, UpPoint));
    acima = normalizar(produto_vetorial(direita, frente));
}

Ray Camera::gerarRaioParaPixel(int linha, int coluna, const Canvas& canvas) const {
    float u = (coluna + 0.5f) / canvas.nCol;       
    float v = 1.0f - (linha + 0.5f) / canvas.nLin; 
    
    return gerarRaio(u, v);
}

Ray Camera::gerarRaio(float u, float v) const {
    u = u - 0.5f;
    v = v - 0.5f;
    
    Ponto pontoJanela = calcularPontoJanela(u, v);
    
    Vetor direcao = normalizar(pontoJanela - eye);
    
    return Ray(eye, direcao);
}

Ponto Camera::calcularPontoJanela(float u, float v) const {
    Ponto centroJanela = (frente * distanciaFocal) + eye;
    
    float deslocX = u * janela.wJ;
    float deslocY = v * janela.hJ;
    
    Ponto pontoJanela = (direita * deslocX) + (acima * deslocY) + centroJanela;
    
    return pontoJanela;
}

// ============== MOVIMENTAÇÃO DA CÂMERA ==============

void Camera::mover(const Vetor& deslocamento) {
    eye = deslocamento + eye;
    AtPoint = deslocamento + AtPoint;
    atualizarSistemaCoordenadas();
}

void Camera::rotacionar(float anguloX, float anguloY, float anguloZ) {
    rotacionarEmY(anguloY);
    
    rotacionarEmX(anguloX);
    
    rotacionarEmZ(anguloZ);
}

void Camera::moverFrente(float distancia) {
    Vetor deslocamento = frente * distancia;
    mover(deslocamento);
}

void Camera::moverDireita(float distancia) {
    Vetor deslocamento = direita * distancia;
    mover(deslocamento);
}

void Camera::moverCima(float distancia) {
    Vetor deslocamento = acima * distancia;
    mover(deslocamento);
}

void Camera::rotacionarEmY(float anguloGraus) {
    float anguloRad = anguloGraus * M_PI / 180.0f;
    
    float cosA = cos(anguloRad);
    float sinA = sin(anguloRad);
    
    Vetor novaFrente;
    novaFrente.x = frente.x * cosA + frente.z * sinA;
    novaFrente.y = frente.y;
    novaFrente.z = -frente.x * sinA + frente.z * cosA;
    
    AtPoint = eye + (novaFrente * distanciaFocal);
    atualizarSistemaCoordenadas();
}

void Camera::rotacionarEmX(float anguloGraus) {
    float anguloRad = anguloGraus * M_PI / 180.0f;
    
    float cosA = cos(anguloRad);
    float sinA = sin(anguloRad);
    
    Vetor novaFrente;
    novaFrente.x = frente.x;
    novaFrente.y = frente.y * cosA - frente.z * sinA;
    novaFrente.z = frente.y * sinA + frente.z * cosA;
    
    novaFrente = normalizar(novaFrente);
    
    float maxPitch = 85.0f * M_PI / 180.0f;
    float pitchAtual = asin(novaFrente.y);
    
    if (fabs(pitchAtual) < maxPitch) {
        AtPoint = eye + (novaFrente * distanciaFocal);
        atualizarSistemaCoordenadas();
    }
}

void Camera::rotacionarEmZ(float anguloGraus) {
    float anguloRad = anguloGraus * M_PI / 180.0f;
    
    float cosA = cos(anguloRad);
    float sinA = sin(anguloRad);
    
    Vetor novoAcima;
    novoAcima.x = acima.x * cosA - acima.y * sinA;
    novoAcima.y = acima.x * sinA + acima.y * cosA;
    novoAcima.z = acima.z;
    
    UpPoint = novoAcima;
    
    atualizarSistemaCoordenadas();
}

void Camera::zoom(float fator) {

    distanciaFocal *= fator;
    
    float proporcao = janela.wJ / distanciaFocal;
    janela.wJ = distanciaFocal * proporcao;
    janela.hJ = distanciaFocal * proporcao;
}