#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define DEMO_SIZE 15

int num_comparacoes;


typedef struct {
    int  id;
    char destino[20];
    int  prazo_dias;
    float peso_kg;
} Pacote;

Pacote deposito[DEMO_SIZE] = {
    { 1, "Sao Paulo",       3, 2.5f},
    { 2, "Rio de Janeiro",  0, 1.2f},  
    { 3, "Belo Horizonte",  7, 4.0f},
    { 4, "Salvador",        1, 0.8f},
    { 5, "Fortaleza",       5, 3.3f},
    { 6, "Curitiba",        0, 2.1f}, 
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



int particionar(Pacote *pacotes, int inicio, int fim) {

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



void imprimir_fila(const Pacote *pacotes, int total) {
    printf("\n  %-4s %-18s %-10s %-10s \n", "ID", "Destino", "Prazo", "Peso(kg)");
    printf("  %-4s %-18s %-10s  %s\n", "----", "------------------", "----------", "----------");

    for (int i = 0; i < total; i++) {
        /* Texto de prazo: mostra "HOJE!" se prazo = 0 */
        char txt_prazo[16];
        if (pacotes[i].prazo_dias == 0)
            snprintf(txt_prazo, sizeof(txt_prazo), "HOJE!");
        else
            snprintf(txt_prazo, sizeof(txt_prazo), "%d dia(s)", pacotes[i].prazo_dias);

        printf("  #%-3d %-18s %-10s %-10.1f \n", pacotes[i].id, pacotes[i].destino, txt_prazo, pacotes[i].peso_kg);
    }
}



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



    printf("============================================================\n");
    printf("  SISTEMA DE ENTREGAS — QuickSort por Prazo de Entrega\n");
    printf("============================================================\n");
    

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

  
    return 0;
}
