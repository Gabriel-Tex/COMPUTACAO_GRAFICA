#include "auxiliares/Renderizacao.h"
#include <iostream>
#include <cmath>

bool Renderiza::encontrarIntersecaoMaisProxima(const Ray& ray,
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

bool Renderiza::verificarSombra(const Ponto& ponto,
                                     const Vetor& normal,
                                     const Vetor& direcaoLuz,
                                     const std::vector<std::unique_ptr<Objeto>>& objetos,
                                     float distanciaLuz,
                                     float EPS) {
    
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

Cor Renderiza::calcularIluminacaoComSombra(const Ponto& ponto,
                                                const Vetor& normal,
                                                const Ray& raioOriginal,
                                                const Propriedades& props,
                                                const IluminacaoCena& iluminacao,
                                                const std::vector<std::unique_ptr<Objeto>>& objetos,
                                                int material,
                                                int idObjeto,
                                                float EPS) {
    
    Vetor L = iluminacao.ponto_da_fonte - ponto;
    float distanciaLuz = comprimento(L);
    L = normalizar(L);
    
    IluminacaoCena iluminacaoAjustada = iluminacao;
    
    if (verificarSombra(ponto, normal, L, objetos, distanciaLuz, EPS)) {
        iluminacaoAjustada.intensidade_da_fonte = Cor(0, 0, 0);
    }
    
    return CorResultante(ponto, normal, raioOriginal, props, 
                        iluminacaoAjustada, material, idObjeto);
}

Cor Renderiza::calcularIluminacaoComSombraeTextura(const Ponto& ponto,
                                                     const Vetor& normal,
                                                     const Ray& raioOriginal,
                                                     const Propriedades& props,
                                                     const IluminacaoCena& iluminacao,
                                                     const std::vector<std::unique_ptr<Objeto>>& objetos,
                                                     int material,
                                                     float EPS) {
    
    Vetor L = iluminacao.ponto_da_fonte - ponto;
    float distanciaLuz = comprimento(L);
    L = normalizar(L);
    
    IluminacaoCena iluminacaoAjustada = iluminacao;
    
    if (verificarSombra(ponto, normal, L, objetos, distanciaLuz, EPS)) {
        iluminacaoAjustada.intensidade_da_fonte = Cor(0, 0, 0);
    }
    
    return CorResultante(ponto, normal, raioOriginal, props, 
                        iluminacaoAjustada, material, 0);
}

Objeto* Renderiza::encontrarObjetoMaisProximo(const Ray& ray,
                                                   const std::vector<std::unique_ptr<Objeto>>& objetos,
                                                   float& t_min,
                                                   Ponto& ponto_int,
                                                   float EPS) {
    
    t_min = -1.0f;
    Objeto* obj_mais_proximo = nullptr;
    
    for (const auto& obj : objetos) {
        float t;
        if (obj->intersecta(ray, t) && t > EPS) {
            if (t_min < 0 || t < t_min) {
                t_min = t;
                obj_mais_proximo = obj.get();
            }
        }
    }
    
    if (obj_mais_proximo != nullptr) {
        ponto_int = ray.P_0 + (ray.dr * t_min);
    }
    
    return obj_mais_proximo;
}

Cor Renderiza::calcularCorFinal(const IntersecaoResultado& intersecao,
                                     const Ray& raioOriginal,
                                     const IluminacaoCena& iluminacao,
                                     const std::vector<std::unique_ptr<Objeto>>& objetos,
                                     float EPS) {
    
    Vetor normal = intersecao.objeto->calcularNormal(intersecao.ponto);
    
    if (intersecao.objeto->temTextura()) {
        Cor corTextura = intersecao.objeto->getCorTextura(intersecao.ponto);
        
        Propriedades propsAjustadas = intersecao.objeto->getPropriedades();
        propsAjustadas.Kdif = corTextura;
        propsAjustadas.Kamb = corTextura * 0.3f; 
        
        return calcularIluminacaoComSombraeTextura(intersecao.ponto, normal, raioOriginal,
                                               propsAjustadas, iluminacao, objetos,
                                               intersecao.objeto->getMaterial(), EPS);
    }
    else {
        return calcularIluminacaoComSombra(intersecao.ponto, normal, raioOriginal,
                                          intersecao.objeto->getPropriedades(), iluminacao, objetos,
                                          intersecao.objeto->getMaterial(), 
                                          intersecao.objeto->getId(), EPS);
    }
}

void Renderiza::listarObjetos(const std::vector<std::unique_ptr<Objeto>>& objetos) {
    std::cout << "Cena com " << objetos.size() << " objetos:" << std::endl;
    for (size_t i = 0; i < objetos.size(); i++) {
        std::cout << "  " << i << ". " << objetos[i]->getNome() 
                  << " (ID: " << objetos[i]->getId() << ")" << std::endl;
    }
}