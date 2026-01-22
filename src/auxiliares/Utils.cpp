#include "auxiliares/Utils.h"
#include <iostream>
#include <cmath>

bool Utils::encontrarIntersecaoMaisProxima(const Ray& ray,
                                                    const std::vector<std::unique_ptr<Objeto>>& objetos,
                                                    IntersecaoResultado& resultado,
                                                    float EPS) {
    
    resultado.reset();
    
    for (const auto& obj : objetos) {
        float t_temp;
        if (obj->intersecta(ray, t_temp) && t_temp > EPS) {
            if (t_temp < resultado.t) {
                resultado.t = t_temp;
                resultado.objeto = obj.get();
            }
        }
    }
    
    if (resultado.objeto != nullptr) {
        resultado.ponto = ray.P_0 + (ray.dr * resultado.t);
        return true;
    }
    return false;
}

bool Utils::verificarSombra(const Ponto& ponto,
                                     const Vetor& normal,
                                     const Vetor& direcaoLuz,
                                     const std::vector<std::unique_ptr<Objeto>>& objetos,
                                     float distanciaLuz,
                                     float EPS) {
    
    // Ponto levemente deslocado para evitar auto-interseção
    Ponto pontoSombra = ponto + (normal * EPS);
    Ray shadowRay(pontoSombra, direcaoLuz);
    
    for (const auto& obj : objetos) {
        float t;
        if (obj->intersecta(shadowRay, t) && t > EPS && t < distanciaLuz) {
            return true;
        }
    }
    return false;
}

Cor Utils::calcularIluminacaoComSombra(const Ponto& ponto,
                                                const Vetor& normal,
                                                const Ray& raioOriginal,
                                                const Propriedades& props,
                                                const IluminacaoCena& iluminacao,
                                                const std::vector<std::unique_ptr<Objeto>>& objetos,
                                                int material,
                                                int idObjeto,
                                                float EPS) {
    
    // Vetor para a fonte de luz
    Vetor L = iluminacao.ponto_da_fonte - ponto;
    float distanciaLuz = comprimento(L);
    L = normalizar(L);
    
    // Copiar iluminação para ajustar
    IluminacaoCena iluminacaoAjustada = iluminacao;
    
    // Verificar sombras
    if (verificarSombra(ponto, normal, L, objetos, distanciaLuz, EPS)) {
        iluminacaoAjustada.intensidade_da_fonte = Cor(0, 0, 0);
    }
    
    // Usar sua função CorResultante existente
    return CorResultante(ponto, normal, raioOriginal, props, 
                        iluminacaoAjustada, material, idObjeto);
}

void Utils::listarObjetos(const std::vector<std::unique_ptr<Objeto>>& objetos) {
    std::cout << "Cena com " << objetos.size() << " objetos:" << std::endl;
    for (const auto& obj : objetos) {
        std::cout << "  - " << obj->getNome() << " (ID: " << obj->getId() << ")" << std::endl;
    }
}