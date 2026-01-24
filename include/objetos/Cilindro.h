#ifndef CILINDRO_H
#define CILINDRO_H

#include "Objeto.h"
#include "./auxiliares/Matrizes.h"
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
    int id;
    
public:
    Cilindro(Ponto CB, float rbCil, float altura, Vetor dcil, Propriedades prop, int m);
    
    bool intersecta(const Ray& ray, float& t) const override;
    Vetor calcularNormal(const Ponto& ponto) const override;
    Propriedades getPropriedades() const override;
    int getMaterial() const override;
    std::string getNome() const override;
    int getId() const override;
    
    Cor getCorTextura(const Ponto& ponto) const override;
    bool temTextura() const override;
    void setTextura(Textura* tex) override;
    
    Ponto getCentroBase() const { return CB; }
    float getRaio() const { return rbCil; }
    float getAltura() const { return altura; }
    Vetor getDirecao() const { return dcil; }
    Ponto getCentroMedio() const { return CB + (dcil * (altura/2)); }
    
    bool verificarAltura(const Ponto& ponto) const;
    Ponto calcularTopo() const;

    void transforma(const Matriz4x4& M);
    void transladar(float tx, float ty, float tz);
    void escalar(float sx, float sy, float sz, Ponto ponto_fixo = Ponto(0,0,0));
    void rotacionarX(float angulo);
    void rotacionarY(float angulo);
    void rotacionarZ(float angulo);
    void cisalharX_XZ(float angulo);
    void cisalharY_XY(float angulo);
    void cisalharY_XZ(float angulo);
    void cisalharZ_XY(float angulo);
    void espelharXY();
    void espelharXZ();
    void espelharYZ();
    void rotacionarEmEixoArbitrario(const Vetor& eixo, float anguloGraus, Ponto ponto);
    
};

#endif