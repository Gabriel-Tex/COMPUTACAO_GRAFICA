#ifndef OBJETOCOMPLEXO_H
#define OBJETOCOMPLEXO_H

#include "Objeto.h"
#include "./auxiliares/Matrizes.h"
#include <vector>
#include <memory>

using namespace std;

class ObjetoComplexo : public Objeto {
private:
    vector<Objeto*> componentes;
    mutable Objeto* componenteAtingido; 
    vector<unique_ptr<Objeto>> componentesOwned;
    Propriedades props;
    int materialId;
    
public:
    ObjetoComplexo();
    
    bool intersecta(const Ray& ray, float& ti) const override;
    Vetor calcularNormal(const Ponto& ponto) const override;
    string getNome() const override;
    int getId() const override;
    Cor getCorTextura(const Ponto& ponto) const override;
    bool temTextura() const override; 
    void setTextura(Textura* tex) override;
    Propriedades getPropriedades() const override;
    int getMaterial() const override;
    
    void adicionarComponente(Objeto* obj); 
    
    void transforma(const Matriz4x4& M);
    
};

#endif