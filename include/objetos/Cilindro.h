#ifndef CILINDRO_H
#define CILINDRO_H

#include "Objeto.h"
#include "./auxiliares/Textura.h"
#include <string>

#define M_PI 3.14159265358979323846

class Cilindro : public Objeto {
private:
    Ponto CB;
    float rbCil;
    float altura;
    Vetor dcil;
    Propriedades prop;
    int m;
    Textura* textura;
    bool temTexturaFlag;
    static int nextId;
    int id;
    
public:
    // Construtor mantendo compatibilidade
    Cilindro(Ponto CB, float rbCil, float altura, Vetor dcil, Propriedades prop, int m);
    
    // Métodos da interface Objeto
    bool intersecta(const Ray& ray, float& t) const override;
    Vetor calcularNormal(const Ponto& ponto) const override;
    Propriedades getPropriedades() const override;
    int getMaterial() const override;
    std::string getNome() const override;
    int getId() const override;
    
    // Métodos de textura
    Cor getCorTextura(const Ponto& ponto) const override;
    bool temTextura() const override;
    void setTextura(Textura* tex) override;
    
    // Getters específicos
    Ponto getBase() const { return CB; }
    float getRaio() const { return rbCil; }
    float getAltura() const { return altura; }
    Vetor getDirecao() const { return dcil; }
    
    // Funções auxiliares (para compatibilidade)
    bool verificarAltura(const Ponto& ponto) const;
    Ponto calcularTopo() const;
};

#endif