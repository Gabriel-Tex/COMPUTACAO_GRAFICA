#ifndef TEXTURA_H
#define TEXTURA_H

#include "Pontos.h"
#include "Vetores.h"
#include "Iluminacao.h"
#include <vector>
#include <string>

// ============== DEFINIÇÃO DA CLASSE ==============
class Textura {
private:
    int width, height, channels;
    std::vector<unsigned char> data;
    bool carregada;
    
public:
    Textura();
    ~Textura();
    
    // ============== MÉTODOS ASSOCIOADOS À TEXTURA ==============

    // Carrega uma textura de arquivo
    bool carregar(const std::string& caminho);
    
    // Amostra a textura nas coordenadas (u,v) [0,1]
    Cor amostrar(float u, float v) const;
    
    // Amostra com repetição (tiling)
    Cor amostrarRepetir(float u, float v) const;
    
    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool estaCarregada() const { return carregada; }
};

#endif