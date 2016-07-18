#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SRV_PORT    1332
#define SRV_ADDR    "127.0.0.1"
#define CNT_LISTEN  16
#define MSG_MAXLEN  256


int main(int argc, void *argv[])
{
    int                 sock;               // Сокет клиента    
    struct sockaddr_in  addr;               // Структура для bind
    char                msg[MSG_MAXLEN];
    pthread_t           tid;
    
    // Создаем сокет
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        perror("socket");
        exit(-1);
    }
    
    // Заполняем структуру и биндим
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SRV_PORT);
    addr.sin_addr.s_addr = inet_addr(SRV_ADDR);

    // Подключаемся к серверу
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
        perror("connect");
        close(sock);
        exit(-1);
    }

    while (1){
        // Считываем и отправляем
        printf("send: ");
        scanf("%s", msg);
        if (!strcmp(msg, "exit"))
            break;
        send(sock, msg, strlen(msg) + 1, 0);

        //Принимаем
        if (recv(sock, msg, MSG_MAXLEN, 0))
            printf("recv: %s\n", msg);
    }

    // Завершение
    close(sock);
    exit(0);
}
