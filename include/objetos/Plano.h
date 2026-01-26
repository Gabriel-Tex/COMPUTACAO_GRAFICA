#ifndef PLANO_H
#define PLANO_H

#include "Objeto.h"
#include "./auxiliares/Matrizes.h"
#include "./auxiliares/Textura.h"
#include <string>

class Plano : public Objeto {
private:
    Ponto P_pi;
    Vetor n;
    Propriedades prop;
    int m;
    Textura* textura;
    bool temTexturaFlag;
    int id;
    float escalaTextura; 
    
public:
    Plano(Ponto P, Vetor N, Propriedades propriedades, int m);
    bool intersecta(const Ray& ray, float& ti) const override;
    Vetor calcularNormal(const Ponto& ponto) const override;
    Propriedades getPropriedades() const override;
    int getMaterial() const override;
    std::string getNome() const override;
    int getId() const override;
    
    Cor getCorTextura(const Ponto& ponto) const override;
    bool temTextura() const override;
    void setTextura(Textura* tex) override;
    void setEscalaTextura(float escala);
    
    Ponto getPonto() const { return P_pi; }
    Vetor getNormal() const { return n; }
    
    Cor getCorTexturaPt(const Ponto& Pt) const;
    Cor amostrarTexturaCustom(float u, float v) const;

    void transforma(const Matriz4x4& M);
    void transladar(float tx, float ty, float tz);
    void escalar(float sx, float sy, float sz, Ponto ponto_fixo = Ponto(0,0,0)); 
    void rotacionarX(float angulo);
    void rotacionarY(float angulo);
    void rotacionarZ(float angulo);  
    void espelharXY();
    void espelharXZ();
    void espelharYZ();
    void rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus, Ponto ponto);
};

#endif