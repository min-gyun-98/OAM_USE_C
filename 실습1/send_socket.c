// 소켓 통신으로 메시지 보내기

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
	int s_socket, c_socket;
	struct sockaddr_in s_addr;
	struct sockaddr_in c_addr;
	int len;
	char send_message[100];

	s_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_port = htons(9000);

	if (bind(s_socket, (struct sockaddr*)&s_addr, sizeof(s_addr)) == -1)
	{
		perror("bind 실패");
		exit(1);
	}

	if (listen(s_socket, 5) == -1) {
		perror("listen 실패");
		exit(1);
	}
	printf("Server Ready\n");

	len = sizeof(c_addr);
	if ((c_socket = accept(s_socket, (struct sockaddr*)&c_addr, &len)) == -1)
	{
		perror("accept 실패");
		exit(1);
	}

	printf("Connect Success\n");

	printf("보낼 메시지를 입력하세요 : ");
	fgets(send_message, 100, stdin);

	write(c_socket, send_message, strlen(send_message));

	close(s_socket);
	close(c_socket);

	return 0;
}