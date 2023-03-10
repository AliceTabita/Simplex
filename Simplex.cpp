#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstring>
using namespace std;

typedef struct Vetor
{
    int length = 0;
    int max_len = 0;
    // comma separated integer
    string temp_str = "";

    int *vetor;

    void instanciar(int tamanho)
    {
        vetor = new int[tamanho];
        this->max_len = tamanho;
    }
    void append(int item)
    {
        if (this->length == this->max_len)
        {
            cout << "Vetor cheio" << endl;
            return;
        }
        this->vetor[this->length] = item;
        this->length++;
    }
    // Converte uma string de números sem separação em vetor [cada digito em uma célula do vetor]
    // is digit serve para verificar se é um numero

    void converter_string_vetor()
    {
        vetor = new int[this->temp_str.size()];
        int indice = 0;
        for (int i = 0; i < this->temp_str.size(); i++)
        {
            if (isdigit(this->temp_str[i]))
            {
                string temp;
                temp.push_back(this->temp_str[i]);
                this->vetor[indice] = stoi(temp);
                indice++;
            }
        }
        this->length = indice;
    }
    void imprime()
    {
        if (this->length == 0)
        {
            cout << "vetor vazio" << endl;
        }
        for (int i = 0; i < this->length; i++)
        {
            cout << this->vetor[i] << endl;
        }
    }
    bool existe(int number)
    {
        if (this->length == 0)
        {
            return false;
        }
        for (int i = 0; i < this->length; i++)
        {
            if (this->vetor[i] == number)
            {
                return true;
            }
        }
        return false;
    }

    bool not_in(int number)
    {
        return !this->existe(number);
    }
    bool in(int number)
    {
        return this->existe(number);
    }
} Vetor;

typedef struct
{
    int nr_linhas = 0;
    int nr_colunas = 0;
    string nm_matriz;
    bool is_iniciada = false;
    float **matriz;
    void liberar_memoria()
    {
        free(this->matriz);
    }

    void insert(int pos, Vetor vec)
    {
        for (int i = 0; i < vec.length; i++)
        {
            this->matriz[pos][i] = vec.vetor[i];
        }
    }
    void instanciar(int linhas, int colunas)
    {

        this->matriz = new float *[linhas];
        for (int i = 0; i < linhas; i++)
        {
            this->matriz[i] = new float[colunas];
        }
        for (int i = 0; i < linhas; i++)
        {
            for (int j = 0; j < colunas; j++)
            {
                this->matriz[i][j] = 0;
            }
        }

        this->nr_linhas = linhas;
        this->nr_colunas = colunas;
        this->is_iniciada = true;
    }
    void imprimir_matriz()
    {
        if (!this->is_iniciada)
        {
            cout << "Matriz " << this->nm_matriz << " ainda não instânciada" << endl;
        }
        else
        {
            cout << "Matriz de " << this->nm_matriz << ":" << endl;
            cout << "Legenda:" << endl;
            cout << "S: sinal(0 : = ; 1 : <= ; 2:>= )" << endl;
            cout << "C: constante" << endl;
            // cout << right << setw(5) << right << setw(5) << "0" << right << setw(5) << "|";
            for (int i = 1; i <= this->nr_colunas - 2; i++)
            {
                cout << right << setw(5) << i << "x" << right << setw(5);
            }
            cout << right << setw(6) << "S" << right << setw(6);
            cout << right << setw(6) << "C" << right << setw(6);
            cout << endl;
            for (int i = 0; i < this->nr_linhas; i++)
            {
                // cout << right << setw(5) << i + 1 << right << setw(5) << "|";
                for (int j = 0; j < this->nr_colunas; j++)
                {
                    cout << setprecision(2);
                    cout << right << setw(6) << this->matriz[i][j] << right << setw(6);
                }
                cout << endl;
            }
        }
    }
    void imprimir_matriz_tablo()
    {
        if (!this->is_iniciada)
        {
            cout << "Matriz " << this->nm_matriz << " ainda não instânciada" << endl;
        }
        else
        {
            cout << "Matriz de " << this->nm_matriz << ":" << endl;
            cout << "Legenda:" << endl;
            cout << "C: constante" << endl;
            cout << "X0: função objetivo" << endl;
            for (int i = 1; i <= this->nr_colunas - 2; i++)
            {
                cout << right << setw(5) << i << "x" << right << setw(5);
            }
            cout << right << setw(6) << "C" << right << setw(6);
            cout << right << setw(6) << "X" << right << setw(6);
            cout << endl;
            for (int i = 0; i < this->nr_linhas; i++)
            {
                for (int j = 0; j < this->nr_colunas; j++)
                {
                    cout << setprecision(2);
                    cout << right << setw(6) << this->matriz[i][j] << right << setw(6);
                }
                cout << endl;
            }
        }
        cout << endl;
    }
} Matriz;

typedef struct
{
    int nr_variavel = 0;
    int nr_restricao = 0;
    bool in_tudo_certo = true;
    string nm_localizacao_arquivo;

    Matriz matriz_ppl;
    Matriz matriz_tablo;

    bool is_min = 0;
    int indice_col = -1;
    int menor_indice_linha = 0;

    void ler_arquivo(string nm_arquivo)
    {
        this->nm_localizacao_arquivo = nm_arquivo;

        this->matriz_tablo.nm_matriz = "Tablo";
        this->matriz_ppl.nm_matriz = "Problema de programação linear";
        string nr_variavel, nr_restricao, linha_lida, sinal;

        int negativo = 0;
        cout << "Iniciando leitura do arquivo " << nm_arquivo << endl;
        ifstream arq(nm_arquivo, ios::in); // nm_arquivo é o caminho para o arquivo

        if (!arq)
        {
            cout << "Arquivo .txt nao pode ser aberto" << endl;
            abort();
        }
        getline(arq, sinal);
        if (sinal == "-")
        {
            is_min = 1;
        }
        cout << "Lendo nr_variavel e nr_restricao" << endl;
        getline(arq, nr_variavel);
        getline(arq, nr_restricao);

        cout << "Numero de variaveis: " << nr_variavel << " "
             << "numero de restrições: " << nr_restricao << endl;

        this->nr_restricao = stoi(nr_restricao);
        this->nr_variavel = stoi(nr_variavel);

        this->matriz_ppl.instanciar(this->nr_restricao + 1, this->nr_variavel + 2);

        int linha = 0, coluna = 0;
        cout << "Lendo dados e gerando da matriz do problema" << endl;
        // busca os vertices no arquivo e coloca em uma matriz numero-nr_restricaos x 2
        getline(arq, linha_lida);
        string temp_string;
        for (char &caracter : linha_lida)
        {
            if (linha > this->matriz_ppl.nr_linhas)
            {
                cout << "Houve uma falha ao tentar traduzir o problema." << endl;
                cout << "Erro: O problema possui mais restrições do que o total declarado no número de restrições" << endl;
                this->in_tudo_certo = false;
                return;
            }
            if (caracter == 45)
            {
                negativo = 1;
            }
            else if (caracter != ',' && caracter != ';')
            {
                // Cria string temporaria para poder converter o valor do char lido para inteiro.
                temp_string.push_back(caracter);
            }
            else if (caracter != ';')
            {
                this->matriz_ppl.matriz[linha][coluna] = stoi(temp_string);
                temp_string = "";
                if (negativo == 1)
                {
                    this->matriz_ppl.matriz[linha][coluna] = (this->matriz_ppl.matriz[linha][coluna] * (-1));
                    negativo = 0;
                }
                coluna++;
            }
            else if (caracter == ',' && coluna >= matriz_ppl.nr_colunas)
            {
                cout << "erro, numero de váriaveis maior que o informado" << endl;
                this->in_tudo_certo = false;
                return;
            }
            else
            {
                this->matriz_ppl.matriz[linha][coluna] = stoi(temp_string);
                if (negativo == 1)
                {
                    this->matriz_ppl.matriz[linha][coluna] = (this->matriz_ppl.matriz[linha][coluna] * (-1));
                    negativo = 0;
                }
                temp_string = "";
                coluna = 0;
                linha++;
            }
        }

        this->matriz_ppl.imprimir_matriz();
        this->gerar_tablo();
        this->matriz_tablo.imprimir_matriz_tablo();
        executa_simplex();
        this->in_tudo_certo = true;
    }
    void gerar_tablo()
    {
        cout << "gerando tablo" << endl;
        this->matriz_tablo.instanciar(nr_restricao + 1, (nr_variavel + nr_restricao + 2));
        // inserindo os valores a partir da matriz do problema
        for (int i = 0; i < this->matriz_ppl.nr_linhas; i++)
        {
            for (int j = 0; j < this->matriz_ppl.nr_colunas - 1; j++)
            {
                if (j == matriz_ppl.nr_colunas - 2)
                {
                    this->matriz_tablo.matriz[i][j + nr_restricao] = matriz_ppl.matriz[i][j + 1];
                }
                else if (j < nr_variavel)
                {
                    this->matriz_tablo.matriz[i][j] = matriz_ppl.matriz[i][j];
                }
            }
        }
        for (int i = 1; i < matriz_tablo.nr_linhas; i++)
        {
            matriz_tablo.matriz[i][matriz_tablo.nr_colunas - 1] = nr_variavel + i;
        }
        if (!is_min)
        {
            for (int i = 0; i < this->matriz_tablo.nr_colunas; i++)
            {
                this->matriz_tablo.matriz[0][i] = (matriz_tablo.matriz[0][i] != 0) ? matriz_tablo.matriz[0][i] * (-1) : 0;
            }
        }
        for (int i = 0; i < matriz_tablo.nr_linhas - 1; i++)
        {
            this->matriz_tablo.matriz[i + 1][i + nr_variavel] = 1;
        }
    }
    bool verifica_negativo()
    {
        float menor = 0;
        for (int i = 0; i < this->matriz_tablo.nr_colunas - 2; i++)
        {
            if (this->matriz_tablo.matriz[0][i] < menor)
            {
                menor = this->matriz_tablo.matriz[0][i];
                this->indice_col = i;
            }
        }
        if (menor >= 0)
        {
            this->indice_col = -1;
            return false;
        }
        return true;
    }
    bool verifica_menor_linha()
    {
        float divisao = 0;
        float menor = 0;
        menor_indice_linha = -1;
        for (int i = 1; i < this->matriz_tablo.nr_linhas; i++)
        {
            if ((this->matriz_tablo.matriz[i][this->indice_col]) != 0)
            {
                divisao = this->matriz_tablo.matriz[i][this->matriz_tablo.nr_colunas - 2] / (this->matriz_tablo.matriz[i][this->indice_col]);
                if (menor == 0 && divisao > 0)
                {
                    this->menor_indice_linha = i;
                    menor = divisao;
                }
                if (divisao <= menor && divisao > 0 && menor > 0)
                {
                    this->menor_indice_linha = i;
                    menor = divisao;
                };
            }
        }
        if (menor_indice_linha < 0)
        {
            cout << "Solução Ilimitada" << endl;
            return false;
        }
        else
        {
            return true;
        }
        cout << "menor col: " << indice_col << endl;
        cout << "menor linha: " << menor_indice_linha << endl;
    }
    void executa_linha_pivo()
    {
        float divisor = matriz_tablo.matriz[menor_indice_linha][indice_col];
        // cout << "divisor" << divisor << endl;
        for (int i = 0; i < matriz_tablo.nr_colunas - 1; i++)
        {
            matriz_tablo.matriz[menor_indice_linha][i] = matriz_tablo.matriz[menor_indice_linha][i] / divisor;
        }
        matriz_tablo.matriz[menor_indice_linha][matriz_tablo.nr_colunas - 1] = indice_col + 1;
    }
    void executa_linhas_rest()
    {
        float nr_operacao;
        for (int i = 0; i < matriz_tablo.nr_linhas; i++)
        {
            if (i != menor_indice_linha)
            {
                nr_operacao = matriz_tablo.matriz[i][indice_col];
                // cout << "nr operacao" << nr_operacao << endl;
                // cout << "menor indice" << menor_indice_linha << endl;
                for (int j = 0; j < matriz_tablo.nr_colunas - 1; j++)
                {
                    matriz_tablo.matriz[i][j] = matriz_tablo.matriz[i][j] - (nr_operacao * matriz_tablo.matriz[menor_indice_linha][j]);
                }
            }
        }
    }
    void executa_simplex()
    {
        while (this->verifica_negativo() && this->verifica_menor_linha())
        {
            executa_linha_pivo();
            executa_linhas_rest();
            matriz_tablo.imprimir_matriz_tablo();
        }
        if (is_min)
        {
            matriz_tablo.matriz[0][matriz_tablo.nr_colunas - 2] = matriz_tablo.matriz[0][matriz_tablo.nr_colunas - 2] * (-1);
        }
        solucao();
    }
    bool existe_base(int x)
    {
        for (int i = 1; i < matriz_tablo.nr_linhas; i++)
        {
            if (matriz_tablo.matriz[i][matriz_tablo.nr_colunas - 1] == x + 1)
            {
                return true;
            }
        }
        return false;
    }
    void solucao_alternativa()
    {
        verifica_menor_linha();
        executa_linha_pivo();
        executa_linhas_rest();
        cout << "SOLUÇÃO ALTERNATIVA" << endl;
        matriz_tablo.imprimir_matriz_tablo();
    }
    void is_multiplas()
    {
        int col = matriz_tablo.nr_colunas;
        for (int i = 0; i < nr_variavel; i++)
        {
            if (matriz_tablo.matriz[0][i] == 0 && !existe_base(i))
            {
                cout << "Multiplas soluções ótimas: "
                     << "x" << i + 1 << " é nulo em z" << endl;
                this->indice_col = i;
                solucao_alternativa();
            }
        }
    }
    void solucao()
    {
        int col = matriz_tablo.nr_colunas;
        int valX;
        string resultado = "";
        for (int i = 1; i < matriz_tablo.nr_linhas; i++)
        {
            valX = matriz_tablo.matriz[i][col - 1];
            if (valX <= nr_variavel)
            {
                resultado.append(to_string(matriz_tablo.matriz[i][col - 2])).append(" X").append(to_string(valX)).append(" + ");
            }
        }
        resultado.append(" = ").append(to_string(matriz_tablo.matriz[0][col - 2]));
        cout << "Resultado: " << endl;
        cout << resultado << endl;
        is_multiplas();
    }
    // bool restricoes_iguais()
    // {
    //     int nr_col = matriz_ppl.nr_colunas;
    //     int tipo = matriz_ppl.matriz[1][nr_col - 2];
    //     int cont = 0;
    //     bool tudo_igual = true;
    //     for (int i = 2; i < nr_col; i++)
    //     {
    //         if (matriz_ppl.matriz[i][nr_col - 2] != tipo)
    //         {
    //             tudo_igual = false;
    //             cont++;
    //         }
    //     }
    //     return tudo_igual;
    // }
    // void normalizacao()
    // {
    //     if (restricoes_iguais() && matriz_ppl.matriz[1][matriz_ppl.nr_colunas - 2] == 1)
    //     {
    //         gerar_tablo();
    //     }
    // }
    void copiar_matriz(Matriz *copia, Matriz base)
    {
        copia->instanciar(base.nr_linhas, base.nr_colunas);
        for (int i = 0; i < copia->nr_linhas; i++)
        {
            for (int j = 0; j < copia->nr_colunas; j++)
            {
                copia->matriz[i][j] = base.matriz[i][j];
            }
        }
    }
    void liberar_memoria()
    {
        this->matriz_ppl.liberar_memoria();
        this->matriz_tablo.liberar_memoria();
    }

} Tablo;

int main(int argc, char *argv[])
{
    Tablo tablo;
    string resposta;
    cout << "Informe o arquivo .txt e seu caminho se necessário (exemplo.txt))" << endl;
    cin >> resposta;
    tablo.ler_arquivo(resposta);
    return 0;
}