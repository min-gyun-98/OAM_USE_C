// 메시지 큐 이용 데이터 받기

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 16
#define QKEY (key_t)0111

struct msgq_data {
	long type;
	char text[BUFSIZE];
};

struct msgq_data recv_data;

void main()
{
	int qid;
	char rcv_msg[BUFSIZE];

	if ((qid = msgget(QKEY, IPC_CREAT | 0666)) == -1) {
		perror("msgget error");
		exit(1);
	}

	if (msgrcv(qid, &recv_data, BUFSIZE, 0, 0) == -1) {
		perror("msgrcv error");
		exit(1);
	}
	printf("받은 메시지 : %s\n", recv_data.text);

	if (msgctl(qid, IPC_RMID, 0) == -1) {
		perror("msgctl remove error");
	}
}