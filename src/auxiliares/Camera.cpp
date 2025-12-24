#include "auxiliares/Camera.h"
#include <cmath>


Camera::Camera() 
    : olho(0, 0, 0), 
      alvo(0, 0, -1), 
      vetorUp(0, 1, 0),
      distanciaFocal(30.0f),
      janela(60.0f, 60.0f, 30.0f) {
    atualizarSistemaCoordenadas();
}

Camera::Camera(const Ponto& olho, const Ponto& alvo, const Vetor& up, 
               float distanciaFocal, const Janela& janela)
    : olho(olho), alvo(alvo), vetorUp(up), 
      distanciaFocal(distanciaFocal), janela(janela) {
    atualizarSistemaCoordenadas();
}

void Camera::lookAt(const Ponto& novoOlho, const Ponto& novoAlvo, const Vetor& up) {
    olho = novoOlho;
    alvo = novoAlvo;
    vetorUp = up;
    atualizarSistemaCoordenadas();
}

void Camera::setJanela(const Janela& jan) {
    janela = jan;
}

void Camera::setDistanciaFocal(float dist) {
    distanciaFocal = dist;
}

void Camera::atualizarSistemaCoordenadas() {
    frente = normalizar(alvo - olho);
    
    direita = normalizar(produto_vetorial(frente, vetorUp));
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
    
    Vetor direcao = normalizar(pontoJanela - olho);
    
    return Ray(olho, direcao);
}

Ponto Camera::calcularPontoJanela(float u, float v) const {
    Ponto centroJanela = (frente * distanciaFocal) + olho;
    
    float deslocX = u * janela.wJ;
    float deslocY = v * janela.hJ;
    
    Ponto pontoJanela = (direita * deslocX) + (acima * deslocY) + centroJanela;
    
    return pontoJanela;
}

// ============== MOVIMENTAÇÃO DA CÂMERA ==============

void Camera::mover(const Vetor& deslocamento) {
    olho = deslocamento + olho;
    alvo = deslocamento + alvo;
    atualizarSistemaCoordenadas();
}

void Camera::rotacionar(float anguloX, float anguloY) {
    float cosX = cos(anguloX);
    float sinX = sin(anguloX);
    float cosY = cos(anguloY);
    float sinY = sin(anguloY);
    
    Vetor novoFrente;
    novoFrente.x = frente.x * cosY + frente.z * sinY;
    novoFrente.y = frente.y;
    novoFrente.z = -frente.x * sinY + frente.z * cosY;
    
    alvo = (novoFrente * distanciaFocal) + olho;
    
    atualizarSistemaCoordenadas();
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
    // Rotação horizontal (ao redor do eixo Y)
    float anguloRad = anguloGraus * M_PI / 180.0f;
    
    // Rotacionar o vetor frente no plano XZ
    float cosA = cos(anguloRad);
    float sinA = sin(anguloRad);
    
    Vetor novaFrente;
    novaFrente.x = frente.x * cosA + frente.z * sinA;
    novaFrente.y = frente.y;
    novaFrente.z = -frente.x * sinA + frente.z * cosA;
    
    // Atualizar alvo baseado na nova direção
    alvo = olho + (novaFrente * distanciaFocal);
    atualizarSistemaCoordenadas();
}

void Camera::rotacionarEmX(float anguloGraus) {
    // Rotação vertical
    float anguloRad = anguloGraus * M_PI / 180.0f;
    
    // Matriz de rotação em X
    float cosA = cos(anguloRad);
    float sinA = sin(anguloRad);
    
    // Rotacionar frente e up
    Vetor novaFrente;
    novaFrente.x = frente.x;
    novaFrente.y = frente.y * cosA - frente.z * sinA;
    novaFrente.z = frente.y * sinA + frente.z * cosA;
    
    // Normalizar e limitar ângulo 
    novaFrente = normalizar(novaFrente);
    
    // Limitar pitch para evitar gimbal lock
    float maxPitch = 85.0f * M_PI / 180.0f;
    float pitchAtual = asin(novaFrente.y);
    
    if (fabs(pitchAtual) < maxPitch) {
        alvo = olho + (novaFrente * distanciaFocal);
        atualizarSistemaCoordenadas();
    }
}

void Camera::zoom(float fator) {

    distanciaFocal *= fator;
    
    float proporcao = janela.wJ / distanciaFocal;
    janela.wJ = distanciaFocal * proporcao;
    janela.hJ = distanciaFocal * proporcao;
}