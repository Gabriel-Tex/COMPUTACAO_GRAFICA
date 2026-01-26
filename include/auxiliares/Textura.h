#ifndef TEXTURA_H
#define TEXTURA_H

#include "Pontos.h"
#include "Vetores.h"
#include "Iluminacao.h"
#include <vector>
#include <string>

// ============== DEFINIÇÃO DA CLASSE ==============
class Textura {
public:
    Textura();
    ~Textura();
    int width, height, channels;
    std::vector<unsigned char> data;
    bool carregada;
    
    // ============== MÉTODOS ASSOCIOADOS À TEXTURA ==============

    bool carregar(const std::string& caminho);
    
    Cor amostrar(float u, float v) const;
    
    Cor amostrarRepetir(float u, float v) const;
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool estaCarregada() const { return carregada; }
};

#endif