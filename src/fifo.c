#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>

#define MY_STACK_SIZE 1024
#define MY_PRIORITY 0

K_FIFO_DEFINE(fifo);

// Estrutura para armazenar os dados na FIFO
struct data_item_t {
    void *fifo_reserved;  // Campo obrigatório para FIFO
    char data[100];             // Dado que será enviado
};

void read(void) { // Função para enviar a informção
    while (1) {
        struct data_item_t *rx_data = k_fifo_get(&fifo, K_FOREVER); // Criação da estrutura responsável por adquirir e armazenar o dado da FIFO 
        if (rx_data) {
            printk("Received: %s\n", rx_data->data); // Printar na tela o texto da FIFO
            k_msleep(1000); // Esperar 1 segundo até a próxima iteração
        }
    }
}

void send(void) {
    while (1) {
        static struct data_item_t tx_data; // Variável que armazenará o dado 
        strcpy(tx_data.data, "Hello world"); // Dado a ser enviado
        k_fifo_put(&fifo, &tx_data); // Inserir dado na FIFO
        k_msleep(1000); // Esperar 1 segundo até o próximo envio
    }
}

void send2(void) {
    while (1) {
        static struct data_item_t tx_data2; // Variável que armazenará o dado 
        strcpy(tx_data2.data, "world"); // Dado a ser enviado
        k_fifo_put(&fifo, &tx_data2); // Inserir dado na FIFO
        k_msleep(1000); // Esperar 1 segundo até o próximo envio
    }
}

// Abaixo, as threads de envio e leitura dos dados da FIFO
K_THREAD_DEFINE(RX1, MY_STACK_SIZE, read, NULL, NULL, NULL, MY_PRIORITY, 0, 0);
//K_THREAD_DEFINE(RX2, MY_STACK_SIZE, read, NULL, NULL, NULL, MY_PRIORITY, 0, 0);
K_THREAD_DEFINE(TX1, MY_STACK_SIZE, send, NULL, NULL, NULL, MY_PRIORITY, 0, 0);
K_THREAD_DEFINE(TX2, MY_STACK_SIZE, send2, NULL, NULL, NULL, MY_PRIORITY, 0, 0);