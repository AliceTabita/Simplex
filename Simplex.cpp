#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
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
            cout << right << setw(2) << right << setw(2) << "0" << right << setw(2) << "|";
            for (int i = 1; i <= this->nr_colunas - 2; i++)
            {
                cout << right << setw(2) << i << "x" << right << setw(2);
            }
            cout << right << setw(3) << "S" << right << setw(3);
            cout << right << setw(3) << "C" << right << setw(3);
            cout << endl;
            for (int i = 0; i < this->nr_linhas; i++)
            {
                cout << right << setw(2) << i + 1 << right << setw(2) << "|";
                for (int j = 0; j < this->nr_colunas; j++)
                {
                    cout << right << setw(3) << this->matriz[i][j] << right << setw(3);
                }
                cout << endl;
            }
        }
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

    void ler_grafo_de_arquivo(string nm_arquivo)
    {
        this->nm_localizacao_arquivo = nm_arquivo;

        this->matriz_tablo.nm_matriz = "Tablo";
        this->matriz_ppl.nm_matriz = "Problema de programação linear";
        string nr_variavel, nr_restricao, linha_lida;

        cout << "Iniciando leitura do arquivo " << nm_arquivo << endl;
        ifstream arq(nm_arquivo, ios::in); // nm_arquivo é o caminho para o arquivo

        if (!arq)
        {
            cout << "Arquivo .txt nao pode ser aberto" << endl;
            abort();
        }
        cout << "Lendo nr_variavel e nr_restricao" << endl;
        getline(arq, nr_variavel);
        getline(arq, nr_restricao);

        cout << "Numero de variaveis: " << nr_variavel << " "
             << "numero de restrições: " << nr_restricao << endl;

        this->nr_restricao = stoi(nr_restricao);
        this->nr_variavel = stoi(nr_variavel);

        this->matriz_ppl.instanciar(this->nr_restricao + 1, this->nr_variavel + 2);
        cout << "colunas" << matriz_ppl.nr_colunas << endl;
        int linha = 0, coluna = 0;
        cout << "Lendo dados e gerando da matriz do problema" << endl;
        // busca os vertices no arquivo e coloca em uma matriz numero-nr_restricaos x 2
        getline(arq, linha_lida);
        for (char &caracter : linha_lida)
        {
            if (linha > this->matriz_ppl.nr_linhas)
            {
                cout << "Houve uma falha ao tentar traduzir o problema." << endl;
                cout << "Erro: O problema possui mais restrições do que o total declarado no número de restrições" << endl;
                this->in_tudo_certo = false;
                return;
            }
            if (caracter != ',' && caracter != ';')
            {
                // Cria string temporaria para poder converter o valor do char lido para inteiro.
                string temp_string;
                temp_string.push_back(caracter);
                this->matriz_ppl.matriz[linha][coluna] = stoi(temp_string);
            }
            else if (caracter != ';' && coluna < matriz_ppl.nr_colunas)
            {
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
                coluna = 0;
                linha++;
            }
        }

        this->matriz_ppl.imprimir_matriz();
        this->gerar_tablo();
        this->matriz_tablo.imprimir_matriz();
        this->in_tudo_certo = true;
    }
    void gerar_tablo()
    {
        cout << "gerando tablo" << endl;
        this->matriz_tablo.instanciar(nr_restricao + 1, nr_variavel + 1);
        // inserindo os valores a partir da matriz do problema
        for (int i = 0; i < this->matriz_ppl.nr_linhas; i++)
        {
            for (int j = 0; j < this->matriz_ppl.nr_colunas; j++)
            {
                if (j == matriz_ppl.nr_colunas - 2)
                {
                    this->matriz_tablo.matriz[i][j] = matriz_ppl.matriz[i][j + 1];
                }
                else
                {
                    this->matriz_tablo.matriz[i][j] = matriz_ppl.matriz[i][j];
                }
            }
        }
        for (int i = 0; i < this->matriz_tablo.nr_colunas - 1; i++)
        {
            this->matriz_tablo.matriz[0][i] = matriz_tablo.matriz[0][i] * (-1);
        }
    }
    // void verifica_negativo(){
    //     float menor;
    //     for (int i = 0; i < this->matriz_tablo.nr_colunas; i++)
    //     {
    //         if(this->matriz_tablo.matriz[0][i]<0){
    //             if(this->matriz_tablo.matriz[0][i]<menor){
    //                 menor=this->matriz_tablo.matriz[0][i];
    //             }
    //         };
    //     }
    // }

    // void gerar_matriz_adjacencias()
    // {
    //     // cout << "Gerando matriz de adjacencias" << endl;
    //     this->matriz_adj.instanciar(this->nr_variavel, this->nr_variavel);

    //     // insere os valores da matriz de ligação na matriz de adjacências
    //     for (int i = 0; i < this->matriz_ligacao.nr_linhas; i++)
    //     {
    //         // a matriz adjacente no indice q esta sendo lido na matriz de ligação -1 recebe 1
    //         this->matriz_adj.matriz[this->matriz_ligacao.matriz[i][0] - 1][this->matriz_ligacao.matriz[i][1] - 1] = 1;
    //         this->matriz_adj.matriz[this->matriz_ligacao.matriz[i][1] - 1][this->matriz_ligacao.matriz[i][0] - 1] = 1;
    //     }
    //     cout << "Matriz de adjacencias gerada" << endl;
    // }
    // void gerar_matriz_adjacencias_ponderada()
    // {
    //     // cout << "Gerando matriz de adjacencias" << endl;
    //     this->matriz_adj_ponderada.instanciar(this->nr_variavel, this->nr_variavel);

    //     // insere os valores da matriz de ligação na matriz de adjacências
    //     for (int i = 0; i < this->matriz_ligacao_ponderada.nr_linhas; i++)
    //     {
    //         // matriz ponderada
    //         // no indice lido na matriz de ligação -1 eu insiro o peso da nr_restricao daquela ligação
    //         this->matriz_adj_ponderada.matriz[this->matriz_ligacao.matriz[i][0] - 1][this->matriz_ligacao.matriz[i][1] - 1] = this->matriz_ligacao_ponderada.matriz[i][2];
    //         this->matriz_adj_ponderada.matriz[this->matriz_ligacao.matriz[i][1] - 1][this->matriz_ligacao.matriz[i][0] - 1] = this->matriz_ligacao_ponderada.matriz[i][2];
    //     }
    //     cout << "Matriz de adjacencias ponderada gerada" << endl;
    // }
    // void gerar_matriz_incidencias()
    // {
    //     // cout << "Gerando matriz de incidencias" << endl;
    //     this->matriz_inc.instanciar(this->nr_restricao, this->nr_variavel);
    //     // insere os valores da matriz de ligação na matriz de incidências
    //     for (int i = 0; i < this->matriz_ligacao.nr_linhas; i++)
    //     {
    //         this->matriz_inc.matriz[i][this->matriz_ligacao.matriz[i][0] - 1] = 1;
    //         this->matriz_inc.matriz[i][this->matriz_ligacao.matriz[i][1] - 1] = 1;
    //     }
    //     cout << "Matriz de incidencias gerada" << endl;
    // }
    void imprime_grafo()
    {
        if (!this->in_tudo_certo)
        {
            cout << "Grafo com problemas." << endl;
        }
        cout << "Imprimindo Grafo: " << endl;
        cout << "Vertices: " << this->nr_variavel << endl;
        cout << "nr_restricaos: " << this->nr_restricao << endl;

        // this->matriz_ligacao.imprimir_matriz();
        // this->matriz_adj.imprimir_matriz();
        // this->matriz_adj_ponderada.imprimir_matriz();
        // this->matriz_inc.imprimir_matriz();
        // this->matriz_ligacao_ponderada.imprimir_matriz();
        // this->matriz_arvore_minima.imprimir_matriz();
    }
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
        // this->matriz_ligacao.liberar_memoria();
        // this->matriz_ligacao_ponderada.liberar_memoria();
        // this->matriz_adj.liberar_memoria();
        // this->matriz_adj_ponderada.liberar_memoria();
        // this->matriz_arvore_minima.liberar_memoria();
        // this->matriz_inc.liberar_memoria();
    }

} Tablo;

int main(int argc, char *argv[])
{
    Tablo tablo;
    tablo.ler_grafo_de_arquivo("test.txt");
    return 0;
}