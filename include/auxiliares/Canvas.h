#ifndef CANVAS_H
#define CANVAS_H

#include "Pontos.h"
#include "Iluminacao.h"
#include <cstddef> 

// ============== DEFINIÇÃO DA CLASSE ==============
class Canvas
{
public:
    // número de colunas e número de linhas
    size_t nCol, nLin;
    // cor do background
    Cor bgColor;
    Canvas(size_t C, size_t L, Cor background = Cor(0.5f, 0.7f, 1.0f));
};

// ============== MÉTODOS ASSOCIADOS A CANVAS ==============

#endif