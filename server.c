#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#define TCP_PORT            9600
#define MAX_CLIENTS         10
#define RECV_BUFFER_SIZE    10

int clients[MAX_CLIENTS]   = {0};

void *client_handler(void *arg)
{
    long id                             = (long) arg;
    char recv_buffer[RECV_BUFFER_SIZE]  = {0};

    // Receive the UDP port from the client
    while(1)
    {
        recv(clients[id], recv_buffer, sizeof(recv_buffer), 0);
        
        int udp_port = atoi(recv_buffer);
        printf("UDP PORT RECEBIDO (cliente %ld): %d\n", id + 1, udp_port);
        
        if (udp_port == 0)
        {
            printf("Cliente %ld desconectado\n", id + 1);
            send(clients[id], "Desconectado do Servidor", sizeof("Desconectado do Servidor"), 0);

            close(clients[id]);
            clients[id] = 0; // Liberar o slot do cliente
            break;
        }

        int random_number = rand() % 100;
        char random_number_str[10];
        snprintf(random_number_str, sizeof(random_number_str), "%d\n", random_number);

        printf("Número aleatório (cliente %ld): %s\n", id + 1, random_number_str);

        struct sockaddr_in udp_server_addr  = {0};
        udp_server_addr.sin_family          = AF_INET;
        udp_server_addr.sin_port            = htons(udp_port);
        udp_server_addr.sin_addr.s_addr     = INADDR_ANY;

        sendto(clients[id], &random_number_str, strlen(random_number_str), 0, (struct sockaddr *)&udp_server_addr, sizeof(udp_server_addr));
    }

    return NULL;
}

int main()
{
    long client_counter          = 0;
    int server                   = 0;
    int clients_len[MAX_CLIENTS] = {0};

    pthread_t threads[MAX_CLIENTS] = {0};

    struct sockaddr_in caddr[MAX_CLIENTS] = {0};
    struct sockaddr_in saddr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port        = htons(TCP_PORT)
    };

    server = socket(AF_INET, SOCK_STREAM, 0);

    if(server < 0)
    {
        printf("Erro ao criar o socket do servidor (%d)\n", errno);
        exit(-1);
    }

    if(bind(server, (struct sockaddr*)&saddr, sizeof(saddr)) == -1)
    {
        printf("Erro no bind (%d)\n", errno);
        exit(-1);
    }

    if(listen(server, MAX_CLIENTS) < 0)
    {
        printf("Erro no listen (%d)\n", errno);
        exit(-1);
    }

    printf("Servidor criado!\n");

    srand(time(NULL)); // Inicializar a semente do gerador de números aleatórios

    while(client_counter < MAX_CLIENTS)
    {
        clients[client_counter] = accept(server, (struct sockaddr *)&caddr[client_counter], &clients_len[client_counter]);

        if(clients[client_counter] < 0)
        {
            printf("Erro ao estabelecer a conexão\n");
            exit(-1);
        }

        printf("Recebi uma conexão do cliente %ld!\n", client_counter + 1);

        pthread_create(&threads[client_counter], 0, client_handler, (void*)client_counter);
        client_counter++;
    }

    close(server);

    return 0;
}