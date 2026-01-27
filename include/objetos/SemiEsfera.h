#ifndef SEMIESFERA_H
#define SEMIESFERA_H

#include "Objeto.h"
#include <vector>
#include <algorithm>

class SemiEsfera : public Objeto {
private:
    Ponto centro;
    float raio;
    Vetor direcao;
    Propriedades props;
    int m; 
    bool temBase;
    
    Textura* textura;
    bool temTexturaFlag;

public:
    SemiEsfera(Ponto centro, float raio, Vetor direcao, Propriedades props, int materialId, int m, bool temBase = true);

    virtual bool intersecta(const Ray& ray, float& t) const override;
    virtual Vetor calcularNormal(const Ponto& p) const override;
    
    virtual Propriedades getPropriedades() const override { return props; }
    virtual int getMaterial() const override { return m; }
    int getShininess() const { return m; } 
    
    virtual std::string getNome() const override { return "SemiEsfera"; }
    virtual int getId() const override { return 0; }

    virtual bool temTextura() const override { return temTexturaFlag; }
    virtual void setTextura(Textura* tex) override;
    virtual Cor getCorTextura(const Ponto& ponto) const override;

    virtual void transforma(const Matriz4x4& M) override;
    void transladar(float tx, float ty, float tz);
    void escalar(float s, Ponto ponto_fixo); 
    
    void rotacionarX(float anguloGraus);
    void rotacionarY(float anguloGraus);
    void rotacionarZ(float anguloGraus);
    void rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus, Ponto ponto);

    void espelharXY();
    void espelharXZ();
    void espelharYZ();
};

#endif