#include <bits/stdc++.h>
using namespace std;

// definição de um ponto genérico
typedef struct ponto{
    float cx, cy, cz;

    ponto(float x, float y, float z){
        this->cx = x;
        this->cy = y;
        this->cz = z;
    }
} ponto;

// definição de cor
typedef struct cor{
    float r, g, b;

    cor(float r = 0.0f, float g = 0.0f, float b = 0.0f){
        this->r = r;
        this->g = g;
        this->b = b;
    }

} cor;

// definição de um vetor genérico
typedef struct vetor{
    float cx,cy,cz;

    vetor(float x = 0.0f, float y = 0.0f, float z = 0.0f){
        this->cx = x;
        this->cy = y;
        this->cz = z;
    }

} vetor;


// ============================ FUNÇÕES AUXILIARES ============================
// mantém os valores de r,g,b entre 0 e 1
cor normalizaRGB(int r, int g, int b) {
    return cor(r / 255.0f, g / 255.0f, b / 255.0f);
}

// ponto2 - ponto1 = vetor entre ponto2 e ponto 1
vetor operator-(ponto ponto2, ponto ponto1){
    return vetor(ponto2.cx - ponto1.cx, ponto2.cy - ponto1.cy, ponto2.cz - ponto1.cz);
}

// inverter sentido de vetor 
vetor operator-(vetor v)
{
    return vetor(-v.cx, -v.cy, -v.cz);
}

// produto escalar entre dois vetores
float produto_escalar(vetor vetot1, vetor vetot2)
{
    return (vetot1.cx * vetot2.cx + vetot1.cy * vetot2.cy + vetot1.cz * vetot2.cz);
}

// ||v|| = sqrt(x^2 + y^2 + z^2)
// normalizacao: v/||v||
vetor normalizar(vetor v)
{
    float modulo = sqrtf(v.cx * v.cx + v.cy * v.cy + v.cz * v.cz);

    return vetor(v.cx / modulo, v.cy / modulo, v.cz / modulo);
}

float comprimento(vetor v) {
    return sqrtf(v.cx * v.cx + v.cy * v.cy + v.cz * v.cz);
}

cor arroba(cor a, cor b)
{
    return {a.r * b.r, a.g * b.g, a.b * b.b};
}


// ============================ VARIÁVEIS ============================

// janela
float wJanela = 60.0f;
float hJanela = 60.0f; 
float dJanela = 30.0f;
ponto centro_da_janela(0.0f,0.0f,-dJanela);

// olho do pintor
ponto olho_do_pintor(0.0f,0.0f,0.0f);

// ============= esfera =============

float rEsfera = 40.0f;
ponto centro_da_esfera(0.0f,0.0f,-100.0f); 
cor esfColor = normalizaRGB(255, 0, 0);

// propriedades da esfera 
cor Kdif_esfera(0.8f, 0.1f, 0.1f);    
cor Kesp_esfera(0.9f, 0.9f, 0.9f);    
cor Kamb_esfera(0.7f, 0.1f, 0.1f);    
int m_esfera = 50;   

// ============= cilindro =============
ponto CB_cilindro = centro_da_esfera;
float rbCilindro = rEsfera / 3;
float altura_cilindro = 3 * rEsfera;
vetor dcil = normalizar (vetor(-1/sqrt(3), 1/sqrt(3), -1/sqrt(3)));

// propriedades do cilindro
cor Kdif_cilindro ( 0.2, 0.3, 0.8);
cor Kesp_cilindro ( 0.2, 0.3, 0.8);
cor Kamb_cilindro ( 0.2, 0.3, 0.8);
int m_cil = 10;
// ============= cone =============
ponto CB_cone ( altura_cilindro * dcil.cx + CB_cilindro.cx,
                altura_cilindro * dcil.cy + CB_cilindro.cy,
                altura_cilindro * dcil.cz + CB_cilindro.cz);
float rbCone = 1.5f * rEsfera;
float altura_cone = rbCone / 3;
vetor dco = dcil; 

// propriedades do cone
cor Kdif_cone (0.8, 0.3, 0.2);
cor Kesp_cone (0.8, 0.3, 0.2);
cor Kamb_cone (0.8, 0.3, 0.2);
int m_cone = 10;

// ============= plano do chão =============

// ponto conhecido do plano
ponto P_chao (0.0f, -rEsfera, 0.0f);
// vetor unitário normal ao plano
vetor n_chao (0, 1, 0);
// Reflectividade do plano do chão
cor Kdif_chao(0.2f, 0.7f, 0.2f);
cor Kesp_chao(0.0f, 0.0f, 0.0f);
cor Kamb_chao(0.2f, 0.7f, 0.2f);
int m_chao = 1;

// ============= plano de fundo =============

// ponto conhecido do plano
ponto P_fundo(0.0f, 0.0f, -200.0f);  
// vetor unitário normal ao plano
vetor n_fundo(0.0f, 0.0f, 1.0f);     
// Reflectividade do plano do fundo
cor Kdif_fundo(0.3f, 0.3f, 0.7f);
cor Kesp_fundo(0.0f, 0.0f, 0.0f);
cor Kamb_fundo(0.3f, 0.3f, 0.7f);
int m_fundo = 1;

// =========================================

// canvas
size_t nCol = 500;
size_t nLin = 500;

cor bgColor (0.5f, 0.5f, 0.5f);

// ============= Iluminação =============

// intensidade da fonte
cor Intensidade_da_fonte (0.7f, 0.7f, 0.7f);
// intensidade da luz ambiente da cena
cor Intensidade_ambiente_da_cena (0.3f, 0.3f, 0.3f);
// ponto da fonte
ponto ponto_da_fonte (0.0f, 60.0f, -30.0f);

// ============================ FUNÇÕES DE INTERSEÇÃO ============================
bool Intersecao_raio_e_esfera(vetor dr, ponto origem, float &ti){
    // vetor que vai da olho_do_pintor ao centro da esfera
    vetor w = origem - centro_da_esfera; 

    // coeficientes da equação da interseção
    float a = produto_escalar(dr, dr);
    float b = 2 * (produto_escalar(w, dr));
    float c2 = produto_escalar(w, w) - rEsfera * rEsfera;

    double delta = b*b - 4*a*c2;

    // se delta negativo, então o raio não intercepta a esfera
    if(delta < 0) return false;

    // calculando parâmetros de distância t1 e t2
    float t1 = (-b + sqrt(delta)) / (2 * a);
    float t2 = (-b - sqrt(delta)) / (2 * a);

    // os pontos de interseção estão na frente do olho
    if (t1 > 0 && t2 > 0) ti = min(t1, t2); // escolhe a mais próxima   
    // apenas P(t1) está na frente do olho
    else if (t1 > 0) ti = t1; 
    // apenas P(t2) está na frente do olho  
    else if (t2 > 0) ti = t2;
    // nem P(t1) nem P(t2) são observáveis   
    else return false; 
    // delta é não negativo e pelo menos um ponto de interseção é observável
    return true;
}



bool Intersecao_raio_e_plano(vetor dr, ponto origem, ponto P_pi, float &ti, vetor n){
    // t = - (w * n) / (dr * n)
    float denominador = produto_escalar(dr, n);

    // denominador prox. de zero -> raio paralelo ao plano
    if (fabs(denominador) < 1e-6) {
        return false;
    }

    // -w
    vetor w = P_pi - origem;
    // calculando parâmetro t de distância 
    float t = produto_escalar(w, n) / denominador;
    // se t for positivo, a interseção é válida
    if(t > 0) {
        ti = t; 
        return true;}
    return false;
}

bool Intersecao_raio_e_cilindro(vetor dr, ponto origem, float &t) {
    // Vetor do centro da base para a origem do raio
    vetor w = origem - CB_cilindro;
    
    // Projeções nos eixos do cilindro
    float a = produto_escalar(dr, dr) - pow(produto_escalar(dr, dcil), 2);
    float b = 2 * (produto_escalar(dr, w) - produto_escalar(dr, dcil) * produto_escalar(w, dcil));
    float c = produto_escalar(w, w) - pow(produto_escalar(w, dcil), 2) - rbCilindro * rbCilindro;
    
    float delta = b*b - 4*a*c;
    if (delta < 0) return false;
    
    float t1 = (-b - sqrt(delta)) / (2*a);
    float t2 = (-b + sqrt(delta)) / (2*a);
    
    // Verificar se as interseções estão dentro da altura do cilindro
    vector<float> ts_validos;
    
    if (t1 > 0) {
        ponto P1 (  origem.cx + dr.cx * t1,
                    origem.cy + dr.cy * t1,
                    origem.cz + dr.cz * t1  );
        float h1 = produto_escalar(P1 - CB_cilindro, dcil);
        if (h1 >= 0 && h1 <= altura_cilindro) ts_validos.push_back(t1);
    }
    
    if (t2 > 0) {
        ponto P2 (  origem.cx + dr.cx * t2,
                    origem.cy + dr.cy * t2,
                    origem.cz + dr.cz * t2  );

        float h2 = produto_escalar(P2 - CB_cilindro, dcil);
        if (h2 >= 0 && h2 <= altura_cilindro) ts_validos.push_back(t2);
    }
    
    // Verificar tampas
    // tampa inferior
    vetor n_base = -dcil; // Normal aponta para fora
    float t_base;
    if (Intersecao_raio_e_plano(dr, origem, CB_cilindro, t_base, n_base)) {
        ponto P_base (  origem.cx + dr.cx * t_base,
                        origem.cy + dr.cy * t_base,
                        origem.cz + dr.cz * t_base  );
        if (comprimento(P_base - CB_cilindro) <= rbCilindro) {
            ts_validos.push_back(t_base);
        }
    }
    
    // tampa superior
    ponto centro_topo ( altura_cilindro * dcil.cx + CB_cilindro.cx,
                        altura_cilindro * dcil.cy + CB_cilindro.cy,
                        altura_cilindro * dcil.cz + CB_cilindro.cz );

    float t_topo;
    if (Intersecao_raio_e_plano(dr, origem, centro_topo, t_topo, dcil)) {
        ponto P_topo (  origem.cx + dr.cx * t_topo,
                        origem.cy + dr.cy * t_topo,
                        origem.cz + dr.cz * t_topo  );
        if (comprimento(P_topo - centro_topo) <= rbCilindro) {
            ts_validos.push_back(t_topo);
        }
    }
    
    if (ts_validos.empty()) return false;
    
    t = *min_element(ts_validos.begin(), ts_validos.end());
    return true;
}

bool Intersecao_raio_e_cone(vetor dr, ponto origem, float &t) {
    ponto vertice ( CB_cone.cx + dco.cx * altura_cone,
                    CB_cone.cy + dco.cy * altura_cone,
                    CB_cone.cz + dco.cz * altura_cone); // Vértice do cone
    float razao = rbCone / altura_cone; // tan(θ)
    
    vetor w = origem - vertice;
    
    float a = produto_escalar(dr, dr) - (1 + razao*razao) * pow(produto_escalar(dr, dco), 2);
    float b = 2 * (produto_escalar(dr, w) - (1 + razao*razao) * produto_escalar(dr, dco) * produto_escalar(w, dco));
    float c = produto_escalar(w, w) - (1 + razao*razao) * pow(produto_escalar(w, dco), 2);
    
    float delta = b*b - 4*a*c;
    if (delta < 0) return false;
    
    float t1 = (-b - sqrt(delta)) / (2*a);
    float t2 = (-b + sqrt(delta)) / (2*a);
    
    vector<float> ts_validos;
    
    // Verificar interseções com a superfície lateral
    if (t1 > 0) {
        ponto P1 (  origem.cx + dr.cx * t1,
                    origem.cy + dr.cy * t1,
                    origem.cz + dr.cz * t1  );
        float h1 = produto_escalar(P1 - CB_cone, dco);
        if (h1 >= 0 && h1 <= altura_cone) ts_validos.push_back(t1);
    }
    
    if (t2 > 0) {
        ponto P2 (  origem.cx + dr.cx * t2,
                    origem.cy + dr.cy * t2,
                    origem.cz + dr.cz * t2  );
        float h2 = produto_escalar(P2 - CB_cone, dco);
        if (h2 >= 0 && h2 <= altura_cone) ts_validos.push_back(t2);
    }
    
    // Verificar base do cone
    float t_base;
    if (Intersecao_raio_e_plano(dr, origem, CB_cone, t_base, -dco)) {
        ponto P_base (  origem.cx + dr.cx * t_base,
                        origem.cy + dr.cy * t_base,
                        origem.cz + dr.cz * t_base  );
        if (comprimento(P_base - CB_cone) <= rbCone) {
            ts_validos.push_back(t_base);
        }
    }
    
    if (ts_validos.empty()) return false;
    
    t = *min_element(ts_validos.begin(), ts_validos.end());
    return true;
}

vetor normal_cilindro(ponto P) {
    // Calcular a altura ao longo do eixo do cilindro
    float h = produto_escalar(P - CB_cilindro, dcil);
    
    // Verificar se está na tampa inferior (usando tolerância mais inteligente)
    if (h >= -1e-4 && h <= 1e-4) {
        return -dcil;
    }
    
    // Verificar se está na tampa superior
    ponto centro_topo(
        CB_cilindro.cx + dcil.cx * altura_cilindro,
        CB_cilindro.cy + dcil.cy * altura_cilindro,
        CB_cilindro.cz + dcil.cz * altura_cilindro
    );
    
    float h_topo = produto_escalar(P - centro_topo, dcil);
    if (h_topo >= -1e-4 && h_topo <= 1e-4) {
        return dcil;
    }
    
    // Se chegou aqui, está na superfície lateral
    // Calcular o ponto correspondente no eixo do cilindro
    ponto ponto_no_eixo(
        CB_cilindro.cx + dcil.cx * h,
        CB_cilindro.cy + dcil.cy * h,
        CB_cilindro.cz + dcil.cz * h
    );
    
    // A normal é o vetor do ponto no eixo até o ponto na superfície
    return normalizar(P - ponto_no_eixo);
}


vetor normal_cone(ponto P) {
    ponto vertice ( CB_cone.cx + dco.cx * altura_cone,
                    CB_cone.cy + dco.cy * altura_cone,
                    CB_cone.cz + dco.cz * altura_cone );
    
    // Verificar base
    float h_base = produto_escalar(P - CB_cone, -dco);
    if (fabs(h_base) < 1e-4 && comprimento(P - CB_cone) <= rbCone) {
        return -dco; // Normal da base
    }
    
    // Superfície lateral
    ponto projecao (CB_cone.cx + dco.cx * produto_escalar(P - CB_cone, dco),
                    CB_cone.cy + dco.cy * produto_escalar(P - CB_cone, dco),
                    CB_cone.cz + dco.cz * produto_escalar(P - CB_cone, dco));
    vetor lateral = P - vertice;
    //vetor eixo = CB_cone - vertice;
    
    // Projeção no plano perpendicular ao eixo
    vetor proj (lateral.cx - dco.cx * produto_escalar(lateral, dco),
                lateral.cy - dco.cy * produto_escalar(lateral, dco),
                lateral.cz - dco.cz * produto_escalar(lateral, dco));

    return normalizar(proj);
}

bool Sombra_no_plano(ponto Pt, int objeto = -1) {
    vetor L = ponto_da_fonte - Pt;
    float distancia = sqrtf(produto_escalar(L, L));
    vetor l = normalizar(L);
    
    ponto origem_deslocada(Pt.cx + l.cx * 1e-6f,
                          Pt.cy + l.cy * 1e-6f,
                          Pt.cz + l.cz * 1e-6f);

    float ti_esfera, ti_chao, ti_fundo, ti_cilindro, ti_cone;
    
    // Objetos 
    if (objeto != 0 && Intersecao_raio_e_esfera(l, origem_deslocada, ti_esfera)) {
        if (ti_esfera > 0 && ti_esfera < distancia) return true;
    }
    
    if (objeto != 3 && Intersecao_raio_e_cilindro(l, origem_deslocada, ti_cilindro)) {
        if (ti_cilindro > 0 && ti_cilindro < distancia) return true;
    }
    
    if (objeto != 4 && Intersecao_raio_e_cone(l, origem_deslocada, ti_cone)) {
        if (ti_cone > 0 && ti_cone < distancia) return true;
    }
    
    // Planos 
    if (objeto != 1 && Intersecao_raio_e_plano(l, origem_deslocada, P_chao, ti_chao, n_chao)) {
        if (ti_chao > 0 && ti_chao < distancia) return true;
    }
    
    if (objeto != 2 && Intersecao_raio_e_plano(l, origem_deslocada, P_fundo, ti_fundo, n_fundo)) {
        if (ti_fundo > 0 && ti_fundo < distancia) return true;
    }
    
    return false;
}

cor CorResultante(ponto Pt, vetor n, vetor dr, cor Kdif, cor Kesp, cor Kamb, int m, int objeto) {
    cor Intensidade_ambiente_refletida = arroba(Kamb, Intensidade_ambiente_da_cena);
    
    // verificar se ponto está em sombra
    if (Sombra_no_plano(Pt, objeto)) {
        return Intensidade_ambiente_refletida;  
    }
    
    // vetor na direção da fonte
    vetor l = normalizar(ponto_da_fonte - Pt);
    // vetor viwer na direção do observador
    vetor v = -dr;
    // Fd = l · n = cos(θ), e θ < 90°
    float Fd = max(0.0f, produto_escalar(l, n));

    // ========== I_dif = IF @ Kdif * Fd ==========
    // I_dif = IF @ Kdif
    cor Intensidade_difusa = arroba(Intensidade_da_fonte, Kdif);
    // I_dif = I_dif * Fd
    Intensidade_difusa.r = Intensidade_difusa.r * Fd;
    Intensidade_difusa.g = Intensidade_difusa.g * Fd;
    Intensidade_difusa.b = Intensidade_difusa.b * Fd;
    
    // apenas calcular especular se Kesp não for zero
    cor Intensidade_especular(0.0f, 0.0f, 0.0f);
    if (Kesp.r > 0 || Kesp.g > 0 || Kesp.b > 0) {
        // vetor reflexão 
        vetor r = normalizar(vetor( 2 * Fd * n.cx - l.cx, 
                                    2 * Fd * n.cy - l.cy, 
                                    2 * Fd * n.cz - l.cz ));
        
        // Fe = v · r = cos(θ), e θ < 90°
        float Fe_parcial = max(0.0f, produto_escalar(v, r));

        // ========== I_esp = IF @ Kesp * Fe ==========
        // I_esp = IF @ Kesp
        Intensidade_especular = arroba(Intensidade_da_fonte, Kesp);
        // Fe = (v * r)^m
        float Fe = pow(Fe_parcial, m);
        // I_esp = I_esp * Fe
        Intensidade_especular.r = Intensidade_especular.r * Fe;
        Intensidade_especular.g = Intensidade_especular.g * Fe;
        Intensidade_especular.b = Intensidade_especular.b * Fe;
    }
    
    // Internsidade que chega ao observador
    cor Intensidade_observada;
    Intensidade_observada.r = min(1.0f, Intensidade_ambiente_refletida.r + Intensidade_difusa.r + Intensidade_especular.r);
    Intensidade_observada.g = min(1.0f, Intensidade_ambiente_refletida.g + Intensidade_difusa.g + Intensidade_especular.g);
    Intensidade_observada.b = min(1.0f, Intensidade_ambiente_refletida.b + Intensidade_difusa.b + Intensidade_especular.b);
    
    return Intensidade_observada;
}

int main(){
    
    // matriz do canvas
    vector<vector<cor>> canvas(nLin, vector<cor>(nCol, bgColor));

    // dimensões dos retângulos da tela de mosquito
    float Dx = wJanela/nCol;
    float Dy = hJanela/nLin;

    // laço principal que percorre a matriz do canvas
    for (size_t l = 0; l < nLin; l++){
        for (size_t c = 0; c < nCol; c++){
            
            // coordenadas do ponto do centro de um retângulo
            ponto centro_do_pixel(  -wJanela/2 + Dx/2  + c*Dx,
                                    hJanela/2 - Dy/2 - l*Dy,
                                    -dJanela  );
            
            // vetor raio
            vetor ray = centro_do_pixel - olho_do_pintor;
            // raio normalizado
            vetor dr = normalizar(ray);

            // parâmetro de distância do ponto de interseção P(t)
            float ti_esfera, ti_chao, ti_fundo, ti_cilindro, ti_cone;
            
            // interseções nos objetos
            bool hit_esfera = Intersecao_raio_e_esfera(dr, olho_do_pintor, ti_esfera);
            bool hit_chao = Intersecao_raio_e_plano(dr, olho_do_pintor, P_chao, ti_chao, n_chao );
            bool hit_fundo = Intersecao_raio_e_plano(dr, olho_do_pintor, P_fundo, ti_fundo, n_fundo);
            bool hit_cilindro = Intersecao_raio_e_cilindro(dr, olho_do_pintor, ti_cilindro);
            bool hit_cone = Intersecao_raio_e_cone(dr, olho_do_pintor, ti_cone);
            // encontrar a interseção mais próxima
            float ti_min = -1.0f; // ti inicialmente inválido
            int objeto_mais_proximo = -1; // -1: nenhum, 0: esfera, 1: chão, 2: fundo
            
            // verificando em qual objeto o raio bateu primeiro
            if (hit_esfera && ti_esfera > 0 && (ti_min < 0 || ti_esfera < ti_min)) {
                // se bateu na esfera com um ti válido e ti_esfera é o menor ti positivo, então
                ti_min = ti_esfera;
                objeto_mais_proximo = 0;
            }
            if (hit_chao && ti_chao > 0 && (ti_min < 0 || ti_chao < ti_min)) {
                ti_min = ti_chao;
                objeto_mais_proximo = 1;
            }
            if (hit_fundo && ti_fundo > 0 && (ti_min < 0 || ti_fundo < ti_min)) {
                ti_min = ti_fundo;
                objeto_mais_proximo = 2;
            }
            if (hit_cilindro && ti_cilindro > 0 && (ti_min < 0 || ti_cilindro < ti_min)) {
                ti_min = ti_cilindro;
                objeto_mais_proximo = 3; // ID para cilindro
            }

            if (hit_cone && ti_cone > 0 && (ti_min < 0 || ti_cone < ti_min)) {
                ti_min = ti_cone;
                objeto_mais_proximo = 4; // ID para cone
}

            if (objeto_mais_proximo != -1) {
                ponto Pt (  olho_do_pintor.cx + ti_min * dr.cx, 
                            olho_do_pintor.cy + ti_min * dr.cy, 
                            olho_do_pintor.cz + ti_min * dr.cz  );
                
                // propriedades do objeto
                vetor normal;
                cor Kdif, Kesp, Kamb;
                int m;
                
                switch (objeto_mais_proximo) {
                    case 0: // esfera
                        normal = normalizar(Pt - centro_da_esfera);
                        Kdif = Kdif_esfera; Kesp = Kesp_esfera; Kamb = Kamb_esfera;
                        m = m_esfera;
                        break;
                    case 1: // chão
                        normal = n_chao;
                        Kdif = Kdif_chao; Kesp = Kesp_chao; Kamb = Kamb_chao;
                        m = m_chao;
                        break;
                    case 2: // fundo
                        normal = n_fundo;
                        Kdif = Kdif_fundo; Kesp = Kesp_fundo; Kamb = Kamb_fundo;
                        m = m_fundo;
                        break;
                    case 3: // cilindro
                        normal = normal_cilindro(Pt);
                        Kdif = Kdif_cilindro; Kesp = Kesp_cilindro; Kamb = Kamb_cilindro;
                        m = m_cil;
                        break;
                    case 4: // cone
                        normal = normal_cone(Pt);
                        Kdif = Kdif_cone; Kesp = Kesp_cone; Kamb = Kamb_cone;
                        m = m_cone;
                        break;
                }
                canvas[l][c] = CorResultante(Pt, normal, dr, Kdif, Kesp, Kamb, m, objeto_mais_proximo);

            }
        }
    }

    // gerar aquivo ppm
    ofstream arquivo("cena_cilindro_cone_esfera.ppm");
    arquivo << "P3\n" << nCol << " " << nLin << "\n255\n";
    for (size_t l = 0; l < nLin; l++)
    {
        for (size_t c = 0; c < nCol; c++)
        {
            arquivo << canvas[l][c].r * 255 << " " << canvas[l][c].g * 255 << " " << canvas[l][c].b * 255 << " ";
        }
        arquivo << "\n";
    }
    arquivo.close();
    cout << "Imagem gerada." << endl;

    return 0;
}