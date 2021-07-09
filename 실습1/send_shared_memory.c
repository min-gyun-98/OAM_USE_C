// 문자열 전송

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SHMSIZE 1024
#define SHMKEY  (key_t)0111

main()
{
    int shmid, len;
    void *shmaddr;
    char str[20];

    if ((shmid = shmget(SHMKEY, SHMSIZE,IPC_CREAT|0666)) == -1) {
        perror ("shmget failed");
        exit (1); 
    }

    if ((shmaddr = shmat(shmid, NULL, 0)) == (void *)-1) {
        perror ("shmat failed");
        exit (1);
    }

    printf("공유메모리에 보낼 문자열 입력 : ");
	fgets(str, 21, stdin);
    strcpy((char *)shmaddr, str);
    if (shmdt(shmaddr) == -1) {
        perror ("shmdt failed");
        exit (1);
    }
}
