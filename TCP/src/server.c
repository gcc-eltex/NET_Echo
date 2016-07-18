#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SRV_PORT    1332
#define SRV_ADDR    "127.0.0.1"
#define CNT_LISTEN  16
#define MSG_MAXLEN  256

int main(int argc, void *argv[])
{
    int                 sock_srv;       // Сокет сервера
    int                 sock_clt;       // Сокет сервера
    struct sockaddr_in  addr_srv;       //
    struct sockaddr_in  addr_clt;       //
    socklen_t           addr_size;      // Размер структуры addr
    char                msg[MSG_MAXLEN];
    int                 msg_size;

    // Создаем сокет
    sock_srv = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_srv == -1){
        perror("socket");
        exit(-1);
    }
    
    // Заполняем структуру и биндим
    addr_srv.sin_family = AF_INET;
    addr_srv.sin_port = htons(SRV_PORT);
    addr_srv.sin_addr.s_addr = inet_addr(SRV_ADDR);
    if (bind(sock_srv, (struct sockaddr *)&addr_srv, sizeof(addr_srv)) == -1){
        perror("bind");
        exit(-1);
    }

    // Принимаем запрос на подключение
    listen(sock_srv, CNT_LISTEN);
    addr_size = sizeof(addr_clt);
    sock_clt = accept(sock_srv, (struct sockaddr *)&addr_clt, &addr_size);
    if (sock_clt == -1){
        perror("accept");
        exit(-1);
    }
    printf("Connected: %d\n", sock_clt);

    // Пересылаем обратно все полученные сообщения
    while (1)
    {
        msg_size = recv(sock_clt, msg, MSG_MAXLEN, 0);
        if (msg_size == 0)
            break;
        if (msg_size == -1)
            perror("recv");
        else
            send(sock_clt, msg, msg_size, 0);
    }

    // Завершение
    close(sock_srv);
    exit(0);
}
