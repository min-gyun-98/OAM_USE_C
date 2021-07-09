// 소켓 통신으로 메시지 받기

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
	int c_socket;
	struct sockaddr_in addr;
	char recv_message[100];

	c_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	if (connect(c_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		perror("connect 실패");
	}

	int len;
	if ((len = read(c_socket, recv_message, 100)) == -1)
	{
		perror("read 실패");
		exit(1);
	}
	printf("받은 메시지 : %s", recv_message);

	close(c_socket);
	return 0;
}