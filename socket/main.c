#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <string.h>
#pragma comment(lib,"Ws2_32")

#define MYPORT 6666
#define BACKLOG 10
#define MAXDATASIZE 100

int main() {
	int sockfd, new_fd;

	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;

	int sin_size, numberbytes;
	char msg[10], buf[MAXDATASIZE];

	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))== -1) {
		printf("socket error\n");
		exit(1);
	}
	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1) {
		printf("bind error\n");
		closesocket(sockfd);
		exit(1);
	}

	//printf("Hello World\n");
	if (listen(sockfd, BACKLOG) == -1) {
		printf("listen error\n");
		closesocket(sockfd);
		exit(1);
	}
	//printf("111");
	printf("listen ...");

	//等待客户端连接
	sin_size = sizeof(struct sockaddr_in);
	if ((new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size)) == -1) {
		printf("accept error\n");
		closesocket(sockfd);
		exit(1);
	}
	printf("\naccept!\n");

	while (1) {
		memset(buf, 0, sizeof(buf));
		if ((numberbytes = recv(new_fd, buf, MAXDATASIZE, 0)) == -1) {
			continue;
		}
		if (!strcmp(buf, "bye")) {
			closesocket(sockfd);
			closesocket(new_fd);
			return 0;
		}
		printf("%s %d\n", buf, strlen(buf));
		sprintf(msg, "%d", strlen(buf));

		if (send(new_fd, msg, MAXDATASIZE, 0) == -1) {
			printf("send error\n");
			closesocket(sockfd);
			closesocket(new_fd);
			return 0;
		}
		printf(msg);
	}
}