/*
 * APLICACAO: Sistema de Entregas - Ordenar Pacotes por Prazo
 *
 * Problema:
 *   Um entregador tem uma pilha de pacotes em ordem aleatoria.
 *   Cada pacote tem um prazo de entrega (em dias). O entregador
 *   precisa entregar primeiro os pacotes com prazo mais curto
 *   para nao atrasar nenhuma entrega.
 *
 * Solucao:
 *   Usar QuickSort para ordenar os pacotes pelo prazo (crescente).
 *   Prazo 0 = hoje. Prazo 1 = amanha. Prazo 5 = daqui 5 dias.
 *   Depois de ordenar, o entregador pega do topo da pilha — sempre
 *   o mais urgente primeiro.
 *
 * Por que QuickSort?
 *   Media O(n log n). Para 10.000 pacotes, e muito mais rapido
 *   que Bubble Sort O(n^2). Ordena no proprio array, sem memoria extra.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEMO_SIZE 15

static int num_comparacoes;

/* -----------------------------------------------------------------------
 * STRUCT: Pacote
 *
 * Representa um pacote no deposito.
 *
 *   id           — numero do pacote (ex: #001)
 *   destino      — cidade de destino
 *   prazo_dias   — quantos dias faltam para o prazo (0 = hoje)
 *   peso_kg      — peso do pacote em kg
 * ----------------------------------------------------------------------- */
typedef struct {
    int  id;
    char destino[20];
    int  prazo_dias;
    float peso_kg;
} Pacote;

/* -----------------------------------------------------------------------
 * Pacotes fixos para o exemplo legivel.
 * Chegaram ao deposito em ordem aleatoria (como na vida real).
 * Apos ordenar, prazo 0 deve estar no topo, prazo maior no final.
 * ----------------------------------------------------------------------- */
Pacote deposito[DEMO_SIZE] = {
    { 1, "Sao Paulo",       3, 2.5f},
    { 2, "Rio de Janeiro",  0, 1.2f},  /* prazo hoje! */
    { 3, "Belo Horizonte",  7, 4.0f},
    { 4, "Salvador",        1, 0.8f},
    { 5, "Fortaleza",       5, 3.3f},
    { 6, "Curitiba",        0, 2.1f},  /* prazo hoje! */
    { 7, "Manaus",         10, 6.7f},
    { 8, "Porto Alegre",    2, 1.5f},
    { 9, "Recife",          1, 0.5f},
    {10, "Goiania",         8, 5.0f},
    {11, "Brasilia",        4, 3.8f},
    {12, "Belem",           6, 2.2f},
    {13, "Florianopolis",   2, 1.9f},
    {14, "Vitoria",         9, 4.4f},
    {15, "Natal",           3, 0.7f},
};


/* =======================================================================
 * FUNCTION: particionar
 *
 * Escolhe um elemento "pivo" e reorganiza o array de forma que:
 *   - Pacotes com prazo <= pivo ficam a ESQUERDA (mais urgentes)
 *   - Pacotes com prazo >  pivo ficam a DIREITA  (menos urgentes)
 *   - O pivo fica na sua posicao final correta
 *
 * Ordenamos CRESCENTE (menor prazo primeiro), pois quem vence menos
 * dias tem mais urgencia.
 *
 * A troca e feita diretamente aqui com uma variavel temporaria:
 *   Pacote temp = pacotes[i];   <- salva pacotes[i] em temp
 *   pacotes[i]  = pacotes[j];   <- sobrescreve pacotes[i] com pacotes[j]
 *   pacotes[j]  = temp;         <- coloca o valor original de pacotes[i] em pacotes[j]
 * Isso e o mesmo que uma funcao trocar() faria, apenas escrito no lugar.
 *
 * Exemplo visual para [3, 0, 7, 1]:
 *   Pivo = 1 (ultimo elemento)
 *   j=0: pacote[0].prazo=3, nao <= 1, ignora
 *   j=1: pacote[1].prazo=0, 0 <= 1, troca -> [0, 3, 7, 1]
 *   j=2: pacote[2].prazo=7, nao <= 1, ignora
 *   Coloca pivo: [0, 1, 7, 3]  <- pivo (1) na posicao correta
 *
 * Retorna: indice final do pivo
 * ======================================================================= */
static int particionar(Pacote *pacotes, int inicio, int fim) {

    /* Pivo = ultimo elemento do subarray atual */
    int prazo_pivo = pacotes[fim].prazo_dias; 

    /* i marca o limite: tudo ate i tem prazo <= pivo */
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        /* Se este pacote e mais urgente (prazo menor ou igual ao pivo),
         * move ele para o lado esquerdo */
        num_comparacoes++;
        if (pacotes[j].prazo_dias <= prazo_pivo)  {
            i++;
            /* Troca pacotes[i] com pacotes[j] diretamente, sem funcao auxiliar */
            Pacote temp  = pacotes[i];
            pacotes[i]   = pacotes[j];
            pacotes[j]   = temp;
        }
    }

    /* Coloca o pivo na posicao correta (troca com o elemento apos a fronteira) */
    Pacote temp        = pacotes[i + 1];
    pacotes[i + 1]     = pacotes[fim];
    pacotes[fim]       = temp;

    return i + 1;
}


/* =======================================================================
 * FUNCTION: quicksort
 *
 * Ordena pacotes[inicio..fim] em ordem CRESCENTE de prazo_dias.
 *
 * PASSO A PASSO:
 *   1. Se subarray tem 0 ou 1 elemento — ja esta ordenado, para.
 *   2. Chama particionar() — encontra posicao correta do pivo (p).
 *   3. Ordena recursivamente lado ESQUERDO  (inicio .. p-1): prazos menores
 *   4. Ordena recursivamente lado DIREITO   (p+1   .. fim):  prazos maiores
 *
 * A recursao divide o problema ao meio ate que cada subarray tenha
 * 1 elemento, momento em que o array inteiro esta ordenado.
 * ======================================================================= */
void quicksort(Pacote *pacotes, int inicio, int fim) {

    /* Caso base: subarray vazio ou com 1 elemento — nada a fazer */
    if (inicio >= fim) return;

    /* Particiona e obtem o indice do pivo na posicao final */
    int idx_pivo = particionar(pacotes, inicio, fim);

    /* Ordena a metade esquerda (prazos menores) */
    quicksort(pacotes, inicio, idx_pivo - 1);

    /* Ordena a metade direita (prazos maiores) */
    quicksort(pacotes, idx_pivo + 1, fim);
}


/* =======================================================================
 * FUNCTION: imprimir_fila
 *
 * Imprime os pacotes em formato de tabela.
 * Mostra barra visual de urgencia: quanto menor o prazo, mais "!" aparecem.
 * ======================================================================= */
void imprimir_fila(const Pacote *pacotes, int total) {
    printf("\n  %-4s %-18s %-10s %-10s  %s\n",
           "ID", "Destino", "Prazo", "Peso(kg)", "Urgencia");
    printf("  %-4s %-18s %-10s %-10s  %s\n",
           "----", "------------------", "----------", "----------", "--------");

    for (int i = 0; i < total; i++) {
        /* Barra de urgencia: prazo 0 = "!!!!!" (maximo), prazo 10+ = "." */
        char barra[12] = "..........";
        int urgencia = 10 - pacotes[i].prazo_dias;
        if (urgencia < 0) urgencia = 0;
        for (int b = 0; b < urgencia; b++) barra[b] = '!';

        /* Texto de prazo: mostra "HOJE!" se prazo = 0 */
        char txt_prazo[16];
        if (pacotes[i].prazo_dias == 0)
            snprintf(txt_prazo, sizeof(txt_prazo), "HOJE!");
        else
            snprintf(txt_prazo, sizeof(txt_prazo), "%d dia(s)", pacotes[i].prazo_dias);

        printf("  #%-3d %-18s %-10s %-10.1f  [%s]\n",
               pacotes[i].id,
               pacotes[i].destino,
               txt_prazo,
               pacotes[i].peso_kg,
               barra);
    }
}




/* =======================================================================
 * FUNCTION: demo_particao_trace
 *
 * Demonstra passo a passo como particionar() move i e j.
 * Usa array fixo [3,0,7,1,5,10,2,1,3] — valores sao prazo_dias.
 * Pivo = ultimo elemento (3). Mostra cada decisao e troca.
 * ======================================================================= */



/* =======================================================================
 * FUNCTION: main
 *
 * Compara QuickSort em 4 estados derivados do deposito:
 *
 *   Aleatorio     — deposito na ordem original (como chegou)
 *   Ordenado      — deposito ja ordenado por prazo (crescente)
 *   Reverso       — deposito em ordem decrescente de prazo
 *   Quase Ord.    — ordenado com 2 trocas aleatorias
 *
 * Metrica: numero de comparacoes feitas pelo particionar().
 * Com n=15, tempo e imperceptivel — comparacoes revelam o comportamento.
 * ======================================================================= */
int main(void) {


    /* ===== Preparar os 4 estados a partir do deposito ===== */

    Pacote estado_aleatorio[DEMO_SIZE];
    Pacote estado_ordenado[DEMO_SIZE];
    Pacote estado_reverso[DEMO_SIZE];
    Pacote estado_quase[DEMO_SIZE];

    /* Aleatorio = deposito original */
    memcpy(estado_aleatorio, deposito, sizeof(deposito));

    /* Ordenado = deposito ordenado por prazo */
    memcpy(estado_ordenado, deposito, sizeof(deposito));
    num_comparacoes = 0;
    quicksort(estado_ordenado, 0, DEMO_SIZE - 1);

    /* Reverso = ordenado invertido */
    memcpy(estado_reverso, estado_ordenado, sizeof(deposito));
    for (int i = 0, j = DEMO_SIZE - 1; i < j; i++, j--) {
        Pacote tmp        = estado_reverso[i];
        estado_reverso[i] = estado_reverso[j];
        estado_reverso[j] = tmp;
    }

    /* Quase Ordenado = ordenado + 2 trocas */
    memcpy(estado_quase, estado_ordenado, sizeof(deposito));
    Pacote tmp;
    tmp = estado_quase[1]; estado_quase[1] = estado_quase[4]; estado_quase[4] = tmp;
    tmp = estado_quase[9]; estado_quase[9] = estado_quase[13]; estado_quase[13] = tmp;

    /* ===== Cabecalho ===== */

    printf("============================================================\n");
    printf("  SISTEMA DE ENTREGAS — QuickSort por Prazo de Entrega\n");
    printf("  Comparacao em 4 estados do deposito (%d pacotes)\n", DEMO_SIZE);
    printf("============================================================\n");
    printf("\nMetrica: comparacoes feitas em particionar().\n");
    printf("Pivo = ultimo elemento. Pior caso em arrays ja ordenados.\n");

    /* ===== Cenarios ===== */

    struct {
        const char *nome;
        const char *descricao;
        Pacote     *dados;
    } cenarios[] = {
        {"ALEATORIO",     "deposito na ordem original",          estado_aleatorio},
        {"ORDENADO",      "deposito crescente por prazo",        estado_ordenado },
        {"REVERSO",       "deposito decrescente por prazo",      estado_reverso  },
        {"QUASE ORDENADO","ordenado com 2 trocas (pos 1-4, 9-13)", estado_quase },
    };
    int num_cenarios = (int)(sizeof(cenarios) / sizeof(cenarios[0]));

    /* Tabela resumo — preenchida apos rodar cada cenario */
    int comparacoes_por_cenario[4];

    for (int s = 0; s < num_cenarios; s++) {
        Pacote copia[DEMO_SIZE];
        memcpy(copia, cenarios[s].dados, sizeof(deposito));

        printf("\n------------------------------------------------------------\n");
        printf("  [%s] %s\n", cenarios[s].nome, cenarios[s].descricao);
        printf("------------------------------------------------------------\n");

        printf("\n  ANTES:\n");
        imprimir_fila(copia, DEMO_SIZE);

        num_comparacoes = 0;
        quicksort(copia, 0, DEMO_SIZE - 1);
        comparacoes_por_cenario[s] = num_comparacoes;

        printf("\n  DEPOIS (%d comparacoes):\n", num_comparacoes);
        imprimir_fila(copia, DEMO_SIZE);
    }

    /* ===== Tabela resumo ===== */

    printf("\n============================================================\n");
    printf("  RESUMO — Comparacoes por tipo de entrada (n=%d)\n", DEMO_SIZE);
    printf("============================================================\n");
    printf("\n  %-20s  %-14s  %s\n", "Tipo", "Comparacoes", "Comportamento");
    printf("  %-20s  %-14s  %s\n",
           "--------------------", "--------------", "-------------------");
    const char *comportamento[] = {
        "O(n log n) esperado",
        "O(n^2) pior caso   ",
        "O(n^2) pior caso   ",
        "O(n log n) aprox.  ",
    };
    for (int s = 0; s < num_cenarios; s++) {
        printf("  %-20s  %-14d  %s\n",
               cenarios[s].nome,
               comparacoes_por_cenario[s],
               comportamento[s]);
    }
    printf("\n  Pior caso teorico  O(n^2): ~%d comparacoes\n",
           DEMO_SIZE * DEMO_SIZE);
    printf("  Melhor caso teorico O(n log n): ~%d comparacoes\n",
           (int)(DEMO_SIZE * 3.9));  /* log2(15) ~ 3.9 */

    return EXIT_SUCCESS;
}
