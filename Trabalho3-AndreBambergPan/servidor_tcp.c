#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_MSG 1024
#define PORTA 2503

//função para realizar as operações matemáticas
float realizar_operacao(char operador, float num1, float num2) {
    switch (operador) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num2 != 0 ? num1 / num2 : 0; //evita divisão por zero
        default: return 0;
    }
}

int main(void) {
    // Variáveis
    int socket_desc, conexao, c;
    struct sockaddr_in server, client;
    char resposta[MAX_MSG];//armazena as mensagens trocadas entre cliente e servidor
    int tamanho;//tamnho da mensagem
    char *client_ip;
    int client_port;

    // Passo 1: Criar o socket do servidor
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Nao foi possivel criar o socket\n");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;//qualquer conexao da maquina
    server.sin_port = htons(PORTA);

    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {//associa o socket a porta e aos ips
        puts("Erro ao fazer bind!!! \n");
        return -1;
    }
    puts("Bind efetuado com sucesso!!! \n");

    // Passo 2: Aguarda por conexões de clientes
    listen(socket_desc, 3);
    puts("Aguardando por conexoes...");
    c = sizeof(struct sockaddr_in);

    //aceitando e tratando conexoes
    while ((conexao = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c))) {
        if (conexao < 0) {
            perror("Erro ao receber conexao\n");
            return -1;
        }

        //pegando IP e porta do cliente
        client_ip = inet_ntoa(client.sin_addr);
        client_port = ntohs(client.sin_port);
        printf("Cliente conectou: %s : [ %d ]\n", client_ip, client_port);

        //lendo dados enviados pelo cliente
        if ((tamanho = read(conexao, resposta, MAX_MSG)) < 0) {
            perror("Erro ao receber dados do cliente: ");
            return -1;
        }

        //\0 para garantir que a resposta seja tratada como string
        resposta[tamanho] = '\0';
        printf("O cliente enviou: %s \n", resposta);

        //analisando a string recebida (formato esperado: "num1 operador num2")
        float num1, num2;
        char operador;
        if (sscanf(resposta, "%f %c %f", &num1, &operador, &num2) == 3) {//extrai os dois numeros e o operador da mensagem
            //realiza a operação
            float resultado = realizar_operacao(operador, num1, num2);
            snprintf(resposta, MAX_MSG, "%.2f", resultado);
        } else {
            snprintf(resposta, MAX_MSG, "Erro: formato invalido.");
        }

        //enviando resposta para o cliente
        write(conexao, resposta, strlen(resposta));

        //encerrar conexão
        close(conexao);
    }

    printf("Servidor finalizado...\n");
    return 0;
}
