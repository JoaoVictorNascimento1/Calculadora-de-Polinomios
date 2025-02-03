#include <iostream>
#include <fstream>
using namespace std;

class Poly {
private:
    int grau;         // Grau do polinômio
    double* a;        // Array de coeficientes

public:
    // Construtor default (polinômio vazio)
    Poly() : grau(-1), a(nullptr) {}

    // Construtor específico
    explicit Poly(int n) {
        if (n < 0) {
            grau = -1;
            a = nullptr;
        } else {
            grau = n;
            a = new double[n + 1];
            for (int i = 0; i < n; ++i) {
                a[i] = 0.0; // Todos coeficientes nulos
            }
            if (n == 0) {
                a[0] = 0.0; // Caso específico para grau 0
            } else {
                a[n] = 1.0; // Coeficiente de maior grau com valor 1.0
            }
        }
    }

    // Construtor de cópia
    Poly(const Poly& p) : grau(p.grau), a(nullptr) {
        if (grau >= 0) {
            a = new double[grau + 1];
            for (int i = 0; i <= grau; ++i) {
                a[i] = p.a[i];
            }
        }
    }

    // Construtor de movimento
    Poly(Poly&& p) : grau(p.grau), a(p.a) {
        p.grau = -1;
        p.a = nullptr;
    }

    // Destrutor
    ~Poly() {
        delete[] a;
    }

    // Sobrecarga do operador de atribuição por cópia
    Poly& operator=(const Poly& p) {
        if (this != &p) {
            delete[] a; // Libera a memória atual

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
    Poly& operator=(Poly&& p) {
        if (this != &p) {
            delete[] a; // Libera a memória atual

            grau = p.grau;
            a = p.a;

            p.grau = -1;
            p.a = nullptr;
        }
        return *this;
    }

    // Método de consulta do grau do polinômio
    int getGrau() const {
        if (grau == 0 && a[0] == 1.0) {
            return 0; // Retorna 0 se o grau for 0 e o coeficiente for 1
        }
        return grau;
    }

    // Método de consulta do coeficiente i-ésimo
    double getCoef(int i) const {
        if (i < 0 || i > grau) {
            return 0.0; // Índice inválido
        }
        return a[i];
    }

    // Método para definir o coeficiente i-ésimo
    void setCoef(int i, double valor) {
        if (i < 0 || i > grau) {
            cout << "Erro: Indice invalido." << endl;
            return;
        }
        if (i == grau && grau > 0 && valor == 0.0) {
            cout << "Erro: O coeficiente de maior grau nao pode ser zero." << endl;
            return;
        }
        a[i] = valor;
    }
    
    // Sobrecarga do operador[]
    double operator[](int i) const {
        return getCoef(i);
    }

    // Método recriar 
    void recriar(int n) {
    // Chama o operador de atribuição que lida com alocação e desalocação
        *this = Poly(n);
    }

    // Sobrecarga operador==
    bool operator==(const Poly& p) const {
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

    // Sobrecarga operador !=
    bool operator!=(const Poly& p) const {
        return !(*this == p);
    }

    // Método empty()
    bool empty() const {
        return grau < 0;
    }

    //Método isZero
    bool isZero() const {
        return grau == 0 && a[0] == 0.0;
    }

    //Método getValor()
    double getValor(double x) const {
        if (empty()) {
            return 0.0;
        }
        double resultado = 0.0;
        double potX = 1.0; // Começa com x^0
        // Calcula o valor do polinômio somando a[i] * x^i
        for (int i = 0; i <= grau; ++i) {
            resultado += a[i] * potX;
            potX *= x; // Incrementa a potência de x (x^i)
        }
     return resultado;
    }

    // Sobrecarga operador ()
    double operator()(double x) const {
        return getValor(x);
    }

    // Sobrecarga operador<<
    friend ostream& operator<<(ostream& os, const Poly& p) {
        if (p.empty()) {
            return os; // Não imprime nada se o polinômio é vazio
        }

        for (int i = p.grau; i >= 0; --i) {
            double coef = p.a[i];

            // Se o coeficiente for zero, pulamos este termo
            if (coef == 0.0) {
                if (i == 0 && p.grau == 0) {
                    os << coef; // Imprime 0 se for o único termo
                }
                continue;
            }

            // Imprime o sinal do coeficiente
            if (coef < 0.0) {
                os << '-';
            } else if (i != p.grau) { // Para o primeiro termo não imprimir o sinal +
                os << '+';
            }

            // Imprime o coeficiente, omitindo 1 e -1, exceto quando i == 0
            if ((coef != 1.0 && coef != -1.0) || i == 0) {
                os << (coef < 0.0 ? -coef : coef); // Exibe o valor absoluto manualmente
            }

            // Imprime '*' antes de 'x' se houver coeficiente (omitindo 1 e -1)
            if (i != 0) { // Só imprime 'x' para termos de grau >= 1
                if (coef != 1.0 && coef != -1.0) {
                    os << '*'; // Imprime '*' entre o coeficiente e 'x'
                }
                os << 'x'; // Sempre imprime o 'x'

                // Imprime o expoente se for maior que 1
                if (i > 1) {
                    os << '^' << i;
                }
            }
        }

        return os;
    }

    // Sobrecarga operador >>
    friend istream& operator>>(istream& is, Poly& p) {
        if (p.empty()) {
            return is; // Se o polinômio for vazio, não faz nada
        }

        for (int i = p.grau; i >= 0; --i) {
            if (i == p.grau) {
                // Para o coeficiente de maior grau, garantimos que não seja zero
                do {
                    cout << "x^" << i << ": ";
                    is >> p.a[i];
                    if (p.a[i] == 0.0 && i != 0) {
                        cout << "O coeficiente do termo de maior grau nao pode ser zero. Tente novamente.\n";
                    }
                } while (p.a[i] == 0.0 && i != 0);
            } else {
                // Para os outros coeficientes, não há restrições
                cout << "x^" << i << ": ";
                is >> p.a[i];
            }
        }

        return is;
    }

    // Método Salvar
    bool salvar(const string& nomeArquivo) const {
    ofstream arquivo(nomeArquivo);
    if (!arquivo) {
        return false; // Retorna false se o arquivo não pôde ser aberto
    }

    // Primeira linha: palavra "POLY" + grau
    arquivo << "POLY " << grau << endl;

    // Segunda linha: os coeficientes de a0 até an (somente se o grau for >= 0)
    if (grau >= 0) {
        bool todosZeros = true;
        for (int i = 0; i <= grau; ++i) {
            if (a[i] != 0) {
                todosZeros = false;
                break;
            }
        }

        // Se todos os coeficientes são zero, define o polinômio como grau 0 com coeficiente zero
        if (todosZeros) {
            arquivo << "0" << endl;
        } else {
            for (int i = 0; i <= grau; ++i) {
                arquivo << a[i];
                if (i != grau) {
                    arquivo << " ";
                }
            }
            arquivo << endl; // Quebra de linha ao final
        }
    } else {
        // Caso o grau seja -1 (ou não haja termos), salva como "0"
        arquivo << "0" << endl;
    }

    return true; // Operação bem-sucedida
}

    // Método Ler
    bool ler(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        return false; // Retorna false se o arquivo não pôde ser aberto
    }

    string header;
    int novoGrau;

    // Leitura da palavra reservada "POLY" e do grau
    arquivo >> header >> novoGrau;

    // Verifica se o arquivo está no formato correto
    if (header != "POLY") {
        return false; // Se não começa com "POLY", arquivo inválido
    }

    // Se o grau for negativo, não há coeficientes a serem lidos
    if (novoGrau < 0) {
        grau = -1; // Polinômio vazio
        delete[] a; // Libera o array de coeficientes
        a = nullptr;
        return true;
    }

    // Armazena temporariamente os coeficientes lidos
    double* coefTemp = new double[novoGrau + 1];
    if (!coefTemp) {
        return false; // Falha na alocação de memória
    }

    // Leitura dos coeficientes do arquivo (do grau 0 até o grau mais alto)
    for (int i = 0; i <= novoGrau; ++i) {
        if (!(arquivo >> coefTemp[i])) {
            delete[] coefTemp; // Libera a memória temporária
            return false; // Arquivo inválido se não conseguir ler todos os coeficientes
        }
    }

    // Verifica se o coeficiente do maior grau é zero
    if (coefTemp[novoGrau] == 0.0) {
        delete[] coefTemp; // Libera a memória temporária
        return false; // Coeficiente de maior grau inválido
    }

    // Verifica se todos os coeficientes são zero, o que torna o polinômio nulo para grau > 0
    bool todosZeros = true;
    for (int i = 0; i <= novoGrau; ++i) {
        if (coefTemp[i] != 0.0) {
            todosZeros = false;
            break;
        }
    }
    if (todosZeros) {
        delete[] coefTemp;
        return false; // Polinômio nulo para grau maior que zero é inválido
    }

    // Se todas as verificações foram bem-sucedidas, substitui o conteúdo do polinômio
    delete[] a; // Libera o array de coeficientes atual
    grau = novoGrau;
    a = coefTemp;

    return true; // Leitura bem-sucedida
}

    // Operador unário - para retornar o negativo de um polinômio
Poly operator-() const {
    if (empty()) {
        return *this; // Se o polinômio é vazio, retorna um polinômio vazio
    }

    Poly resultado(*this); // Cria uma cópia do polinômio atual
    for (int i = 0; i <= grau; ++i) {
        resultado.a[i] = -a[i]; // Inverte o sinal de cada coeficiente
    }
    return resultado;
}

// Operador + para a soma de dois polinômios
Poly operator+(const Poly& outro) const {
    // Verifica se ambos os polinômios são vazios
    if (empty() && outro.empty()) {
        return Poly(); // Retorna um polinômio vazio
    }

    // Grau do resultado será o maior grau entre os dois polinômios
    int grauMax = std::max(grau, outro.grau);
    Poly resultado(grauMax);

    // Soma os coeficientes de cada grau
    for (int i = 0; i <= grauMax; ++i) {
        resultado.a[i] = getCoef(i) + outro.getCoef(i);
    }

    // Reduz o grau se necessário
    resultado.reduzirGrau();
    return resultado;
}

// Operador - para a subtração de dois polinômios
Poly operator-(const Poly& outro) const {
    // Verifica se ambos os polinômios são vazios
    if (empty() && outro.empty()) {
        return Poly(); // Retorna um polinômio vazio
    }

    // Grau do resultado será o maior grau entre os dois polinômios
    int grauMax = std::max(grau, outro.grau);
    Poly resultado(grauMax);

    // Subtrai os coeficientes de cada grau
    for (int i = 0; i <= grauMax; ++i) {
        resultado.a[i] = getCoef(i) - outro.getCoef(i);
    }

    // Reduz o grau se necessário
    resultado.reduzirGrau();
    return resultado;
}

// Função auxiliar para reduzir o grau do polinômio
void reduzirGrau() {
    while (grau > 0 && a[grau] == 0.0) {
        --grau; // Reduz o grau
    }

    // Ajusta o array de coeficientes para o novo grau
    double* novoArray = new double[grau + 1];
    for (int i = 0; i <= grau; ++i) {
        novoArray[i] = a[i];
    }

    delete[] a; // Libera a memória antiga
    a = novoArray; // Atualiza o ponteiro para o novo array
}

// Sobrecarga do operador * para a multiplicação de dois polinômios
Poly operator*(const Poly& outro) const {
    // Verifica se algum dos polinômios é vazio
    if (empty() || outro.empty()) {
        return Poly(); // Polinômio vazio como resultado
    }

    // Verifica se algum dos polinômios é nulo (grau zero com coeficiente zero)
    if (isZero() || outro.isZero()) {
        return Poly(0); // Polinômio nulo (grau 0 com coeficiente 0)
    }

    // O grau do resultado é a soma dos graus dos dois polinômios
    int grauResultado = grau + outro.grau;
    Poly resultado(grauResultado);

    // Inicializa os coeficientes do polinômio resultado com zeros
    for (int i = 0; i <= grauResultado; ++i) {
        resultado.a[i] = 0.0;
    }

    // Multiplica os coeficientes dos dois polinômios
    for (int i = 0; i <= grau; ++i) {
        for (int j = 0; j <= outro.grau; ++j) {
            resultado.a[i + j] += a[i] * outro.a[j];
        }
    }

    return resultado;
}


};