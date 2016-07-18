#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SRV_PORT    1333
#define SRV_ADDR    "127.0.0.1"
#define CNT_LISTEN  16
#define MSG_MAXLEN  256

int main(int argc, void *argv[])
{
    int                 sock;       // Сокет сервера
    struct sockaddr_in  addr_srv;       //
    struct sockaddr_in  addr_clt;       //
    char                msg[MSG_MAXLEN];
    socklen_t           addr_len;
    int                 msg_size;

    // Создаем сокет
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1){
        perror("socket");
        exit(-1);
    }
    
    // Заполняем структуру и биндим
    addr_srv.sin_family = AF_INET;
    addr_srv.sin_port = htons(SRV_PORT);
    addr_srv.sin_addr.s_addr = inet_addr(SRV_ADDR);
    if (bind(sock, (struct sockaddr *)&addr_srv, sizeof(addr_srv)) == -1){
        perror("bind");
        exit(-1);
    }

    // Пересылаем обратно все полученные сообщения
    addr_len = sizeof(addr_clt);
    while (1)
    {
        if(recvfrom(sock, msg, MSG_MAXLEN, 0, (struct sockaddr *)&addr_clt, &addr_len) == -1)
            break;
        sendto(sock, msg, MSG_MAXLEN, 0, (struct sockaddr *)&addr_clt, addr_len);
    }

    // Завершение
    close(sock);
    exit(0);
}
