#include "auxiliares/Textura.h"

#define STB_IMAGE_IMPLEMENTATION
#include "auxiliares/stb_image.h"

#include <iostream>
#include <fstream>
#include <algorithm>

Textura::Textura() : width(0), height(0), channels(0), carregada(false) {}

Textura::~Textura() {
    data.clear();
}

bool Textura::carregar(const std::string& caminho) {
    std::cout << "Carregando textura: " << caminho << std::endl;
    
    unsigned char* img = stbi_load(caminho.c_str(), &width, &height, &channels, 0);
    
    if (!img) {
        std::cerr << "Erro ao carregar textura: " << caminho << std::endl;
        std::cerr << "Erro: " << stbi_failure_reason() << std::endl;
        return false;
    }
    
    std::cout << "Textura carregada: " << width << "x" << height 
              << ", canais: " << channels << std::endl;
    
    size_t tamanho = width * height * channels;
    data.resize(tamanho);
    std::copy(img, img + tamanho, data.begin());
    
    stbi_image_free(img);
    
    carregada = true;
    return true;
}

Cor Textura::amostrar(float u, float v) const {
    if (!carregada || data.empty()) {
        return Cor(1.0f, 0.0f, 1.0f); 
    }
    
    u = std::max(0.0f, std::min(1.0f, u));
    v = std::max(0.0f, std::min(1.0f, v));
    
    int x = static_cast<int>(u * (width - 1));
    int y = static_cast<int>(v * (height - 1));
    
    x = std::max(0, std::min(width - 1, x));
    y = std::max(0, std::min(height - 1, y));
    
    int index = (y * width + x) * channels;
    
    if (index + 2 >= data.size()) {
        return Cor(1.0f, 1.0f, 0.0f); 
    }
    
    float r = data[index] / 255.0f;
    float g = data[index + 1] / 255.0f;
    float b = data[index + 2] / 255.0f;
    
    return Cor(r, g, b);
}

Cor Textura::amostrarRepetir(float u, float v) const {
    u = u - floor(u);
    v = v - floor(v);
    return amostrar(u, v);
}