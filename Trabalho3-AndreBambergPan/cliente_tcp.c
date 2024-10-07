#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MSG 1024
#define PORTA 2503

int main(int argc , char *argv[]){
    // variáveis
    int socket_desc;
    struct sockaddr_in servidor;//informações do servidor(ip e porta)
    char mensagem[MAX_MSG];//mensagem enviada para o servidor
    char resposta_servidor[MAX_MSG];//resposta recebida
    int tamanho;//tamanho da resposta recebida
    char *ipservidor = "127.0.0.1";

    // Passo 1: Criar o socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("Nao foi possivel criar socket\n");
        return -1;    
    }

    // Informações para conectar no servidor
    servidor.sin_addr.s_addr = inet_addr(ipservidor);
    servidor.sin_family = AF_INET;//ipv4
    servidor.sin_port = htons(PORTA);

    // Conectar no servidor
    if (connect(socket_desc , (struct sockaddr *)&servidor , sizeof(servidor)) < 0){
        printf("Nao foi possivel conectar\n");
        return -1;
    }
    printf("Conectado no servidor!!!\n");

    /******* COMUNICAÇÃO - TROCA DE MENSAGENS **************/

    //passo 3: Solicitar ao usuário a operação desejada
    printf("Digite a operação desejada (ex: 5 + 3): ");
    fgets(mensagem, MAX_MSG, stdin);

    //envia mensagem para o servidor
    if (send(socket_desc , mensagem , strlen(mensagem) , 0) < 0){
        printf("Erro ao enviar mensagem\n");
        return -1;
    }

    //recebendo resposta do servidor (resultado da operação)
    if ((tamanho = recv(socket_desc, resposta_servidor, MAX_MSG , 0)) < 0){
        printf("Falha ao receber resposta\n");
        return -1;
    }

    //resultado da operação
    resposta_servidor[tamanho] = '\0';
    printf("Resultado: %s\n", resposta_servidor);

    /*****************************************/
    //encerrar conexão
    close(socket_desc);

    printf("Cliente finalizado com sucesso!\n");
    return 0;
}
