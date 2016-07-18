#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SRV_PORT 	1333
#define SRV_ADDR	"127.0.0.1"
#define CNT_LISTEN 	16
#define MSG_MAXLEN	256


int main(int argc, void *argv[])
{
	int 				sock; 				// Сокет клиента	
	struct sockaddr_in 	addr_srv; 				// Структура для bind
	struct sockaddr_in 	addr_clt;
	char 				msg[MSG_MAXLEN];
	socklen_t           addr_len;

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
	
	addr_len = sizeof(addr_srv);
	while (1){
		// Считываем и отправляем
		printf("send: ");
		scanf("%s", msg);
		if (!strcmp(msg, "exit"))
			break;
		sendto(sock, msg, MSG_MAXLEN, 0, (struct sockaddr *)&addr_srv, addr_len);
		//Принимаем
		if (recvfrom(sock, msg, MSG_MAXLEN, 0, (struct sockaddr *)&addr_srv, &addr_len) == -1)
			break;
		printf("recv: %s\n", msg);
	}

	// Завершение
	close(sock);
	exit(0);
}
