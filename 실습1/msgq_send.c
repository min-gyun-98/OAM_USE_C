//메시지 큐 사용 데이터 보내기

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

struct msgq_data send_data;

void main()
{
	int qid;
	char snd_msg[BUFSIZE];
	
	if((qid = msgget(QKEY, IPC_CREAT|0666)) == -1){
		perror("msgget error");
		exit(1);
	}
	printf("보낼 메시지를 입력하세요 : ");
	fgets(snd_msg, 20, stdin);
	send_data.type = 1;
	sprintf(send_data.text, "%s", snd_msg);
	if(msgsnd(qid, &send_data, strlen(send_data.text),0) == -1){
		perror("msgsnd error");
		exit(1);
	}
} 