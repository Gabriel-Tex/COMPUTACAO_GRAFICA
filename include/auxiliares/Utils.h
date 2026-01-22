#ifndef UTILS_H
#define UTILS_H

#include "objetos/Objeto.h"
#include "auxiliares/Ray.h"
#include "auxiliares/Iluminacao.h"
#include <vector>
#include <memory>
#include <limits>

#define M_PI 3.14159265358979323846

// Estrutura para resultado de interseção
struct IntersecaoResultado {
    float t = std::numeric_limits<float>::max();
    Objeto* objeto = nullptr;
    Ponto ponto;
    
    bool valida() const { return objeto != nullptr && t > 0; }
    void reset() {
        t = std::numeric_limits<float>::max();
        objeto = nullptr;
    }
};

// Funções utilitárias para a cena
class Utils {
public:
    // Encontra a interseção mais próxima
    static bool encontrarIntersecaoMaisProxima(const Ray& ray,
                                              const std::vector<std::unique_ptr<Objeto>>& objetos,
                                              IntersecaoResultado& resultado,
                                              float EPS = 1e-4f);
    
    // Verifica se há sombra
    static bool verificarSombra(const Ponto& ponto,
                               const Vetor& normal,
                               const Vetor& direcaoLuz,
                               const std::vector<std::unique_ptr<Objeto>>& objetos,
                               float distanciaLuz,
                               float EPS = 1e-4f);
    
    // Calcula iluminação com sombras
    static Cor calcularIluminacaoComSombra(const Ponto& ponto,
                                          const Vetor& normal,
                                          const Ray& raioOriginal,
                                          const Propriedades& props,
                                          const IluminacaoCena& iluminacao,
                                          const std::vector<std::unique_ptr<Objeto>>& objetos,
                                          int material,
                                          int idObjeto,
                                          float EPS = 1e-4f);
    
    // Função auxiliar para debug
    static void listarObjetos(const std::vector<std::unique_ptr<Objeto>>& objetos);
};

#endif