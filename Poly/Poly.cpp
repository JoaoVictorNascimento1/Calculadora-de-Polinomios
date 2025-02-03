#include "Poly.h"
#include <iostream>
#include <fstream>
using namespace std;

// Construtor default (polinômio vazio)
Poly::Poly() : grau(-1), a(nullptr) {}

// Construtor específico
Poly::Poly(int n) {
    if (n < 0) {
        grau = -1;
        a = nullptr;
    } else {
        grau = n;
        a = new double[n + 1];
        for (int i = 0; i <= n; ++i) {
            a[i] = 0.0;
        }
        if (n == 0) {
            a[0] = 1.0;
        }
    }
}

// Construtor de cópia
Poly::Poly(const Poly& p) : grau(p.grau), a(nullptr) {
    if (grau >= 0) {
        a = new double[grau + 1];
        for (int i = 0; i <= grau; ++i) {
            a[i] = p.a[i];
        }
    }
}

// Construtor de movimento
Poly::Poly(Poly&& p) : grau(p.grau), a(p.a) {
    p.grau = -1;
    p.a = nullptr;
}

// Destrutor
Poly::~Poly() {
    delete[] a;
}

// Sobrecarga do operador de atribuição por cópia
Poly& Poly::operator=(const Poly& p) {
    if (this != &p) {
        delete[] a;
        grau = p.grau;
        if (grau >= 0) {
            a = new double[grau + 1];
            for (int i = 0; i <= grau; ++i) {
                a[i] = p.a[i];
            }
        } else {
            a = nullptr;
        }
    }
    return *this;
}

// Sobrecarga do operador de atribuição por movimento
Poly& Poly::operator=(Poly&& p) {
    if (this != &p) {
        delete[] a;
        grau = p.grau;
        a = p.a;
        p.grau = -1;
        p.a = nullptr;
    }
    return *this;
}

// Método de consulta do grau do polinômio
int Poly::getGrau() const {
    return grau;
}

// Método de consulta do coeficiente i-ésimo
double Poly::getCoef(int i) const {
    if (i < 0 || i > grau) {
        return 0.0;
    }
    return a[i];
}

// Método para definir o coeficiente i-ésimo
void Poly::setCoef(int i, double valor) {
    if (i < 0 || i > grau) {
        cout << "Erro: Índice inválido." << endl;
        return;
    }
    if (i == grau && valor == 0.0 && grau > 0) {
        cout << "Erro: O coeficiente de maior grau não pode ser zero." << endl;
        return;
    }
    a[i] = valor;
}

// Sobrecarga do operador[]
double Poly::operator[](int i) const {
    return getCoef(i);
}

// Método recriar
void Poly::recriar(int n) {
    *this = Poly(n);
}

// Sobrecarga do operador==
bool Poly::operator==(const Poly& p) const {
    if (grau != p.grau) {
        return false;
    }
    for (int i = 0; i <= grau; ++i) {
        if (a[i] != p.a[i]) {
            return false;
        }
    }
    return true;
}

// Sobrecarga do operador!=
bool Poly::operator!=(const Poly& p) const {
    return !(*this == p);
}

// Método empty()
bool Poly::empty() const {
    return grau < 0;
}

// Método isZero
bool Poly::isZero() const {
    return grau == 0 && a[0] == 0.0;
}

// Método getValor()
double Poly::getValor(double x) const {
    if (empty()) {
        return 0.0;
    }
    double resultado = 0.0;
    double potX = 1.0;
    for (int i = 0; i <= grau; ++i) {
        resultado += a[i] * potX;
        potX *= x;
    }
    return resultado;
}

// Sobrecarga do operador()
double Poly::operator()(double x) const {
    return getValor(x);
}

// Sobrecarga do operador<<
ostream& operator<<(ostream& os, const Poly& p) {
    if (p.empty()) {
        return os;
    }
    for (int i = p.grau; i >= 0; --i) {
        double coef = p.a[i];
        if (coef == 0.0) {
            continue;
        }
        if (coef < 0.0) {
            os << '-';
        } else if (i != p.grau) {
            os << '+';
        }
        if ((coef != 1.0 && coef != -1.0) || i == 0) {
            os << (coef < 0.0 ? -coef : coef);
        }
        if (i != 0) {
            os << 'x';
            if (i > 1) {
                os << '^' << i;
            }
        }
    }
    return os;
}

// Sobrecarga do operador>>
istream& operator>>(istream& is, Poly& p) {
    for (int i = p.grau; i >= 0; --i) {
        cout << "Coeficiente de x^" << i << ": ";
        is >> p.a[i];
    }
    return is;
}

// Método salvar
bool Poly::salvar(const string& nomeArquivo) const {
    ofstream arquivo(nomeArquivo);
    if (!arquivo) {
        return false;
    }
    arquivo << "POLY " << grau << endl;
    for (int i = 0; i <= grau; ++i) {
        arquivo << a[i];
        if (i != grau) {
            arquivo << " ";
        }
    }
    arquivo << endl;
    return true;
}

// Método ler
bool Poly::ler(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        return false;
    }
    string header;
    int novoGrau;
    arquivo >> header >> novoGrau;
    if (header != "POLY" || novoGrau < 0) {
        return false;
    }
    double* coefTemp = new double[novoGrau + 1];
    for (int i = 0; i <= novoGrau; ++i) {
        if (!(arquivo >> coefTemp[i])) {
            delete[] coefTemp;
            return false;
        }
    }
    delete[] a;
    a = coefTemp;
    grau = novoGrau;
    return true;
}

// Sobrecarga do operador unário -
Poly Poly::operator-() const {
    Poly resultado(*this);
    for (int i = 0; i <= grau; ++i) {
        resultado.a[i] = -a[i];
    }
    return resultado;
}

// Sobrecarga do operador +
Poly Poly::operator+(const Poly& outro) const {
    int maxGrau = max(grau, outro.grau);
    Poly resultado(maxGrau);
    for (int i = 0; i <= maxGrau; ++i) {
        resultado.a[i] = getCoef(i) + outro.getCoef(i);
    }
    resultado.reduzirGrau();
    return resultado;
}

// Sobrecarga do operador -
Poly Poly::operator-(const Poly& outro) const {
    return *this + (-outro);
}

// Sobrecarga do operador *
Poly Poly::operator*(const Poly& outro) const {
    if (empty() || outro.empty()) {
        return Poly();
    }
    Poly resultado(grau + outro.grau);
    for (int i = 0; i <= grau; ++i) {
        for (int j = 0; j <= outro.grau; ++j) {
            resultado.a[i + j] += a[i] * outro.a[j];
        }
    }
    resultado.reduzirGrau();
    return resultado;
}

// Método privado reduzirGrau
void Poly::reduzirGrau() {
    while (grau > 0 && a[grau] == 0.0) {
        --grau;
    }
    if (grau == 0 && a[0] == 0.0) {
        grau = -1;
        delete[] a;
        a = nullptr;
    }
}
