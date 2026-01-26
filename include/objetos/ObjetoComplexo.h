#ifndef OBJETOTCOMPLEXO_H
#define OBJETOTCOMPLEXO_H

#include "Objeto.h"
#include "./auxiliares/Matrizes.h"
#include <vector>
#include <string>

class ObjetoComplexo : public Objeto {
protected:
    std::vector<Objeto*> componentes;
    
    mutable Objeto* componenteAtingido; 
    
    Propriedades props;
    int materialId;

public:
    ObjetoComplexo();
    virtual ~ObjetoComplexo() = default;

    virtual bool intersecta(const Ray& ray, float& ti) const override;
    virtual Vetor calcularNormal(const Ponto& ponto) const override;
    virtual Propriedades getPropriedades() const override;
    virtual int getMaterial() const override;
    virtual std::string getNome() const override;
    virtual int getId() const override;
    
    virtual Cor getCorTextura(const Ponto& ponto) const override;
    virtual bool temTextura() const override;
    virtual void setTextura(Textura* tex) override;

    virtual void transforma(const Matriz4x4& M) override;

    void adicionarComponente(Objeto* obj);
    const std::vector<Objeto*>& getComponentes() const { return componentes; }

    void transladar(float tx, float ty, float tz);
    void escalar(float sx, float sy, float sz, Ponto ponto_fixo);
    void rotacionarY(float anguloGraus, Ponto ponto_fixo);
    void rotacionarX(float anguloGraus, Ponto ponto_fixo);
    void rotacionarZ(float anguloGraus, Ponto ponto_fixo);
};

#endif