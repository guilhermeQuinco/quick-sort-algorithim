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
#include <time.h>

#define DEMO_SIZE  15
#define LARGE_SIZE 10000

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
 * FUNCTION: gerar_pacotes_aleatorios
 *
 * Gera `total` pacotes com dados aleatorios para o teste de escala.
 *
 * POR QUE: 15 pacotes e facil de ler, mas pequeno demais para medir
 * o tempo real do algoritmo. Com 10.000 pacotes podemos ver a velocidade
 * do QuickSort comparado a algoritmos mais lentos como Bubble Sort.
 * ======================================================================= */
void gerar_pacotes_aleatorios(Pacote *pacotes, int total) {
    const char *cidades[] = {
        "Sao Paulo", "Rio de Janeiro", "Curitiba", "Salvador",
        "Fortaleza", "Manaus",         "Recife",   "Porto Alegre"
    };

    srand((unsigned int)time(NULL));

    for (int i = 0; i < total; i++) {
        pacotes[i].id         = i + 1;
        pacotes[i].prazo_dias = rand() % 15;        /* 0 a 14 dias */
        pacotes[i].peso_kg    = 0.5f + (rand() % 200) / 10.0f; /* 0.5 a 20.5 kg */
        int cidade_idx = rand() % 8;
        /* snprintf garante que nao ultrapassa o tamanho do campo destino */
        snprintf(pacotes[i].destino, sizeof(pacotes[i].destino),
                 "%s", cidades[cidade_idx]);
    }
}


/* =======================================================================
 * FUNCTION: main
 *
 * Executa dois testes:
 *
 *   DEMO 1 — 15 pacotes fixos e legiveis
 *     Mostra a fila ANTES e DEPOIS da ordenacao para voce verificar
 *     visualmente que os pacotes com "HOJE!" foram para o topo.
 *
 *   DEMO 2 — 10.000 pacotes aleatorios
 *     Mede o tempo de ordenacao em milissegundos e verifica
 *     que o resultado esta correto (prazo nunca diminui da esquerda
 *     para a direita).
 * ======================================================================= */
int main(void) {

    /* ===================== DEMO 1: EXEMPLO LEGIVEL ==================== */

    printf("============================================================\n");
    printf("  SISTEMA DE ENTREGAS — QuickSort por Prazo de Entrega\n");
    printf("============================================================\n");

    printf("\n[ANTES] Pacotes em ordem aleatoria no deposito:\n");
    imprimir_fila(deposito, DEMO_SIZE);

    /* Ordena os pacotes por prazo, do mais urgente para o menos urgente */
    quicksort(deposito, 0, DEMO_SIZE - 1);

    printf("\n[DEPOIS] Pacotes ordenados por prazo (mais urgente primeiro):\n");
    imprimir_fila(deposito, DEMO_SIZE);

    printf("\n>> Pacotes com prazo HOJE estao no topo da fila.\n");
    printf(">> Pacotes com prazo longo ficam para o final.\n");

    /* ===================== DEMO 2: TESTE EM GRANDE ESCALA ============= */

    printf("\n============================================================\n");
    printf("  TESTE EM GRANDE ESCALA — %d pacotes aleatorios\n", LARGE_SIZE);
    printf("============================================================\n");

    Pacote *grande = (Pacote *)malloc(LARGE_SIZE * sizeof(Pacote));
    if (!grande) {
        fprintf(stderr, "ERRO: sem memoria para %d pacotes\n", LARGE_SIZE);
        return EXIT_FAILURE;
    }

    gerar_pacotes_aleatorios(grande, LARGE_SIZE);

    clock_t inicio = clock();
    quicksort(grande, 0, LARGE_SIZE - 1);
    clock_t fim = clock();

    double ms = 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC;

    /* Verifica se a ordenacao esta correta: prazo nunca deve diminuir */
    int correto = 1;
    for (int i = 0; i < LARGE_SIZE - 1; i++) {
        if (grande[i].prazo_dias > grande[i + 1].prazo_dias) {
            correto = 0;
            break;
        }
    }

    printf("\n%d pacotes ordenados em %.3f ms\n", LARGE_SIZE, ms);
    printf("Verificacao: %s\n", correto ? "CORRETO" : "ERRO — nao ordenado!");

    printf("\nPrimeiros 5 (prazo mais curto):\n");
    imprimir_fila(grande, 5);

    printf("\nUltimos 5 (prazo mais longo):\n");
    imprimir_fila(grande + LARGE_SIZE - 5, 5);

    free(grande);
    return EXIT_SUCCESS;
}
