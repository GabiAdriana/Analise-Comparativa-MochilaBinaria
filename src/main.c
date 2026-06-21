#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REP_EXATA 100
#define REPETICOES 10000

volatile int lixo;

typedef struct
{ 
    int peso;
    int utilidade;
} Item;

typedef struct
{
    int peso;
    int utilidade;
    double razao;
} ItemG;

/* =====================================================
   GERAÇÃO DE INSTÂNCIAS
   ===================================================== */

void gerarItens(Item itens[], int n)
{
    for (int i = 0; i < n; i++)
    {
        itens[i].peso = rand() % 50 + 1;
        itens[i].utilidade = rand() % 100 + 1;
    }
    printf("Itens gerados:\n");
    for (int i = 0; i < n; i++) 
    {
        printf("(%d,%d) ",
               itens[i].peso,
               itens[i].utilidade);
    }
}

/* =====================================================
   FORÇA BRUTA (EXATA)
   ===================================================== */

int mochilaExata(Item itens[], int n, int capacidade)
{
    int melhor = 0;

    long long total = 1LL << n;

    for (long long mascara = 0; mascara < total; mascara++)
    {
        int peso = 0;
        int valor = 0;

        for (int i = 0; i < n; i++)
        {
            if (mascara & (1LL << i))
            {
                peso += itens[i].peso;
                valor += itens[i].utilidade;
            }
        }

        if (peso <= capacidade && valor > melhor)
        {
            melhor = valor;
        }
    }

    return melhor;
}

/* =====================================================
   PROGRAMAÇÃO DINÂMICA
   ===================================================== */

int mochilaPD(Item itens[], int n, int capacidade, int vetorItensEscolhidos[])
{
    // 1. Alocação da matriz DP
    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++)
    {
        dp[i] = (int *)calloc(capacidade + 1, sizeof(int));
    }

    // 2. Preenchimento da matriz
    for (int i = 1; i <= n; i++)
    {
        for (int w = 0; w <= capacidade; w++)
        {
            if (itens[i - 1].peso <= w)
            {
                int incluir = itens[i - 1].utilidade + dp[i - 1][w - itens[i - 1].peso];
                int excluir = dp[i - 1][w];
                dp[i][w] = (incluir > excluir) ? incluir : excluir;
            }
            else
            {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    int resposta = dp[n][capacidade];

    // =====================================================
    // RECUPERAÇÃO DOS ITENS (BACKTRACKING)
    // =====================================================
    int w = capacidade;
    
    // Se passamos um vetor válido, salvamos o caminho nele
    if (vetorItensEscolhidos != NULL)
    {
        for (int i = n; i > 0; i--)
        {
            if (dp[i][w] != dp[i - 1][w])
            {
                vetorItensEscolhidos[i - 1] = 1; // Item i-1 foi escolhido
                w -= itens[i - 1].peso;
            }
            else
            {
                vetorItensEscolhidos[i - 1] = 0; // Item i-1 ficou de fora
            }
        }
    }

    // 3. Liberação de memória da tabela
    for (int i = 0; i <= n; i++)
    {
        free(dp[i]);
    }
    free(dp);

    return resposta;
}

/* =====================================================
   GULOSA
   ===================================================== */

int compararRazao(const void *a, const void *b)
{
    ItemG *x = (ItemG *)a;
    ItemG *y = (ItemG *)b;

    if (y->razao > x->razao)
        return 1;

    if (y->razao < x->razao)
        return -1;

    return 0;
}

int mochilaAprox12(Item itens[], int n, int capacidade)
{
    ItemG *v = (ItemG *)malloc(n * sizeof(ItemG));

    for (int i = 0; i < n; i++)
    {
        v[i].peso = itens[i].peso;
        v[i].utilidade = itens[i].utilidade;
        v[i].razao =
            (double)itens[i].utilidade /
            itens[i].peso;
    }

    qsort(v, n, sizeof(ItemG), compararRazao);

    int pesoAtual = 0;
    int valorGuloso = 0;

    for (int i = 0; i < n; i++)
    {
        if (pesoAtual + v[i].peso <= capacidade)
        {
            pesoAtual += v[i].peso;
            valorGuloso += v[i].utilidade;
        }
    }

    int melhorItem = 0;

    for (int i = 0; i < n; i++)
    {
        if (itens[i].peso <= capacidade &&
            itens[i].utilidade > melhorItem)
        {
            melhorItem = itens[i].utilidade;
        }
    }

    free(v);

    return (valorGuloso > melhorItem)
           ? valorGuloso
           : melhorItem;
}

/* =====================================================
   TEMPO
   ===================================================== */

double tempoMs(clock_t inicio, clock_t fim)
{
    return ((double)(fim - inicio))
           * 1000.0
           / CLOCKS_PER_SEC;
}

/* =====================================================
   MAIN
   ===================================================== */

int main()
{
    srand(time(NULL));

    FILE *csv = fopen("results/resultados.csv", "w");

    if (csv == NULL)
    {
        printf("Erro ao criar results/resultados.csv\n");
        return 1;
    }

    fprintf(csv,
            "n_itens,capacidade,algoritmo,valor,tempo_ms\n");

    int tamanhos[] =
    {
        5,
        10,
        15,
        20,
        22,
        24
    };

    int quantidadeTamanhos =
        sizeof(tamanhos) / sizeof(tamanhos[0]);

    for (int t = 0; t < quantidadeTamanhos; t++)
    {
        int n = tamanhos[t];

        Item *itens =
            (Item *)malloc(n * sizeof(Item));

        gerarItens(itens, n);

        int somaPesos = 0;

        for (int i = 0; i < n; i++)
        {
            somaPesos += itens[i].peso;
        }

        int capacidade = somaPesos / 2;

        printf("\n=============================\n");
        printf("N = %d\n", n);
        printf("Capacidade = %d\n", capacidade);

        /* ==================================
           FORÇA BRUTA
           ================================== */

        int valorExato =
            mochilaExata(itens,
                         n,
                         capacidade);

        clock_t inicio = clock();

        for (int r = 0; r < REP_EXATA; r++)
        {
            lixo = mochilaExata(itens,
                                n,
                                capacidade);
        }

        clock_t fim = clock();

        double tempoExata =
            tempoMs(inicio, fim) / REP_EXATA;

        fprintf(csv,
                "%d,%d,Exata,%d,%.6f\n",
                n,
                capacidade,
                valorExato,
                tempoExata);

        printf("Exata  -> Valor=%d Tempo=%.6f ms\n",
               valorExato,
               tempoExata);

        /* ==================================
           PROGRAMAÇÃO DINÂMICA
           ================================== */

        // Criamos um vetor para guardar quais itens foram escolhidos nesta instância
        int *itensEscolhidos = (int *)calloc(n, sizeof(int));

        // Chamada única para auditoria e recuperação dos itens
        int valorPD = mochilaPD(itens, n, capacidade, itensEscolhidos);

        // Imprime os itens selecionados APENAS UMA VEZ por tamanho de N
        printf("Itens escolhidos na PD (ID, Peso, Utilidade): ");
        for (int i = 0; i < n; i++)
        {
            if (itensEscolhidos[i] == 1)
            {
                printf("[%d: P=%d, U=%d] ", i, itens[i].peso, itens[i].utilidade);
            }
        }
        printf("\n");

        // Liberamos o vetor auxiliar após o uso
        free(itensEscolhidos);

        // Medição de tempo: rodando em silêncio passando NULL no vetor
        inicio = clock();

        for (int r = 0; r < REPETICOES; r++)
        {
            lixo = mochilaPD(itens,
                             n,
                             capacidade,
                             NULL);
        }

        fim = clock();

        double tempoPD =
            tempoMs(inicio, fim) / REPETICOES;

        fprintf(csv,
                "%d,%d,PD,%d,%.6f\n",
                n,
                capacidade,
                valorPD,
                tempoPD);

        printf("PD     -> Valor=%d Tempo=%.6f ms\n",
               valorPD,
               tempoPD);

        /* ==================================
           GULOSA
           ================================== */

        int valorGulosa =
            mochilaAprox12(itens,
                          n,
                          capacidade);

        inicio = clock();

        for (int r = 0; r < REPETICOES; r++)
        {
            lixo = mochilaAprox12(itens,
                                 n,
                                 capacidade);
        }

        fim = clock();

        double tempoGulosa =
            tempoMs(inicio, fim) / REPETICOES;

        fprintf(csv,
                "%d,%d,Gulosa,%d,%.6f\n",
                n,
                capacidade,
                valorGulosa,
                tempoGulosa);

        printf("Gulosa -> Valor=%d Tempo=%.6f ms\n",
               valorGulosa,
               tempoGulosa);

        free(itens);
    }

    fclose(csv);

    printf("\n===================================\n");
    printf("Arquivo gerado:\n");
    printf("results/resultados.csv\n");
    printf("===================================\n");

    return 0;
}