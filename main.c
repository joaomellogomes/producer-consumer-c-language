#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
// #include <windows.h>
#include <pthread.h>

#define TAM_BUFFER 5

sem_t empty, full;

// Variáveis de controle
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // controla entrada na região crítica
pthread_cond_t cheio = PTHREAD_COND_INITIALIZER;  // quantos slots estão ocupados no buffer
pthread_cond_t vazio = PTHREAD_COND_INITIALIZER;  // quantos slots estão vazios no buffer

typedef struct s_buff
{
    int buff_dados[TAM_BUFFER]; // Buffer de itens
    int fim;                    // Indica a posição do último item do buffer
                                // (pode funcionar como uma pilha)
} Buffer;

void *produtor(void *buffer)
{
    /* Escrever código do Produtor */
    /* Deve imprimir eventos, indicando número do thread:
       1. "[P - <num do thread>] Produzindo item x"
       2. "[P - <num do thread>] Armazenando item x no buffer"
       3. Alternativo -> "[P - <num do thread>] Buffer cheio. Aguardando ..."
       4. Alternativo -> "[P - <num do thread>] Buffer liberado. Continuando ..."
     */

    Buffer *bufferRef = (Buffer *)buffer;

    for (bufferRef->fim = 0; bufferRef->fim < TAM_BUFFER; bufferRef->fim++)
    {
        printf("\nProduzindo item %d", bufferRef->fim);

        int item = rand();

        pthread_cond_wait(&vazio, &lock);

        pthread_mutex_lock(&lock);

        printf("\nArmazenando item %d no buffer[%d]", item, bufferRef->fim);

        bufferRef->buff_dados[bufferRef->fim] = item;

        pthread_mutex_unlock(&lock);

        pthread_cond_broadcast(&cheio);
    }
}

void *consumidor(void *buffer)
{
    /* Escrever c�digo do Consumidor */
    /* Deve imprimir eventos, indicando n�mero do thread:
       1. "[C - <num do thread>] Retirando item x do buffer"
       2. "[C - <num do thread>] Consumindo item x"
       3. Alternativo -> "[C - <num do thread>] Buffer vazio. Aguardando ..."
       4. Alternativo -> "[C - <num do thread>] Novos itens produzidos. Continuando ..."
     */
}

int main()
{
    // Buffer compartilhado
    Buffer buffer;
    buffer.fim = 0;

    /* Referências para Produtor e Consumidor */
    pthread_t prod_thread;
    pthread_t cons_thread;

    /* Cria thread produtora */
    printf("Criando Produtor\n");
    if (pthread_create(&prod_thread, NULL, produtor, &buffer))
    {
        printf("Erro criando Produtor\n");
        return 1;
    }

    /* Cria thread consumidora */
    printf("Criando Consumidor\n");
    if (pthread_create(&cons_thread, NULL, consumidor, &buffer))
    {
        printf("Erro criando Consumidor\n");
        return 1;
    }

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    printf("Finalizando!\n");
    return 0;
}
