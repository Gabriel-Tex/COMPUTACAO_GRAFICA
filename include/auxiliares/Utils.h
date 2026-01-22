#ifndef UTILS_H
#define UTILS_H

#include "auxiliares/Ray.h"
#include "auxiliares/Iluminacao.h"
#include "objetos/Objeto.h"
#include <vector>
#include <memory>
#include <limits>
#include <string>

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

class Utils {
public:
    static bool encontrarIntersecaoMaisProxima(const Ray& ray,
                                              const std::vector<std::unique_ptr<Objeto>>& objetos,
                                              IntersecaoResultado& resultado,
                                              float EPS = 1e-4f);
    
    static bool verificarSombra(const Ponto& ponto,
                               const Vetor& normal,
                               const Vetor& direcaoLuz,
                               const std::vector<std::unique_ptr<Objeto>>& objetos,
                               float distanciaLuz,
                               float EPS = 1e-4f);
    
    static Cor calcularIluminacaoComSombra(const Ponto& ponto,
                                          const Vetor& normal,
                                          const Ray& raioOriginal,
                                          const Propriedades& props,
                                          const IluminacaoCena& iluminacao,
                                          const std::vector<std::unique_ptr<Objeto>>& objetos,
                                          int material,
                                          int idObjeto,
                                          float EPS = 1e-4f);
    
    static Cor calcularIluminacaoComSombraeTextura(const Ponto& ponto,
                                               const Vetor& normal,
                                               const Ray& raioOriginal,
                                               const Propriedades& props,
                                               const IluminacaoCena& iluminacao,
                                               const std::vector<std::unique_ptr<Objeto>>& objetos,
                                               int material,
                                               float EPS = 1e-4f);
    
    static Objeto* encontrarObjetoMaisProximo(const Ray& ray,
                                             const std::vector<std::unique_ptr<Objeto>>& objetos,
                                             float& t_min,
                                             Ponto& ponto_int,
                                             float EPS = 1e-4f);
    
    static void listarObjetos(const std::vector<std::unique_ptr<Objeto>>& objetos);
    
    static Cor calcularCorFinal(const IntersecaoResultado& intersecao,
                               const Ray& raioOriginal,
                               const IluminacaoCena& iluminacao,
                               const std::vector<std::unique_ptr<Objeto>>& objetos,
                               float EPS = 1e-4f);
};

#endif