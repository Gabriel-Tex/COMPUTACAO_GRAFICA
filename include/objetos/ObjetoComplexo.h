#ifndef OBJETOCOMPLEXO_H
#define OBJETOCOMPLEXO_H

#include "objetos/Objeto.h"
#include <vector>
#include <memory>

using namespace std;

class ObjetoComplexo : public Objeto {
private:
    vector<Objeto*> componentes;
    mutable Objeto* componenteAtingido; 
    vector<unique_ptr<Objeto>> componentesOwned;
    
public:
    ObjetoComplexo();
    
    bool intersecta(const Ray& ray, float& ti) const override;
    Vetor calcularNormal(const Ponto& ponto) const override;
    string getNome() const override;
    int getId() const override;
    Cor getCorTextura(const Ponto& ponto) const override;
    bool temTextura() const override; 
    void setTextura(Textura* tex) override;
    
    void adicionarComponente(unique_ptr<Objeto> obj);
    
};

#endif