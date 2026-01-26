#include "auxiliares/Textura.h"

#define STB_IMAGE_IMPLEMENTATION
#include "auxiliares/stb_image.h"

#include <iostream>
#include <algorithm>

Textura::Textura() : width(0), height(0), channels(0), carregada(false) {}

Textura::~Textura() {
    data.clear();
}

bool Textura::carregar(const std::string& caminho) {
    unsigned char* img = stbi_load(caminho.c_str(), &width, &height, &channels, 0);
    
    if (!img) {
        std::cerr << "Erro ao carregar textura: " << caminho << std::endl;
        return false;
    }
    
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

    u = u - std::floor(u);
    v = v - std::floor(v);

    int x = static_cast<int>(u * (width - 1));
    int y = static_cast<int>(v * (height - 1));
    
    y = (height - 1) - y; 

    int index = (y * width + x) * channels;
    
    float r = data[index] / 255.0f;
    float g = data[index + 1] / 255.0f;
    float b = data[index + 2] / 255.0f;
    
    return Cor(r, g, b);
}

Cor Textura::amostrarRepetir(float u, float v) const {
    u = u - floor(u);
    v = v - floor(v);
    
    u = std::max(0.0f, std::min(1.0f, u));
    v = std::max(0.0f, std::min(1.0f, v));
    
    return amostrar(u, v);
}