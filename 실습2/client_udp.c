// 클라이언트 시스템 UDP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void send_cmd(const char *cmd)
{
	struct sockaddr_in addr = 
	{
		.sin_family  = AF_INET,
		.sin_addr.s_addr = inet_addr("127.0.0.1"),
		.sin_port = htons(9000)
	};
	int addr_len = sizeof addr, recv_len;
	char buf[2048];
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	sendto(fd, cmd, strlen(cmd), 0, (struct sockaddr *)&addr, addr_len);
	recv_len = recvfrom(fd, buf, sizeof buf, 0, (struct sockaddr *)&addr, &addr_len);
	printf("%s\n", buf);
}

int main(int argc, char *argv[])
{
	char cmd[2048] = "";
	int i;
	for(i = 1; i < argc; i++)
	{
		strcat(cmd, argv[i]);
		strcat(cmd, "\n");
	}

	send_cmd(cmd);
}