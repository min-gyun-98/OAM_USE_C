// 문자열 수신

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SHMSIZE 1024
#define SHMKEY (key_t)0111

main()
{
	int shmid, len;
	void* shmaddr;
	
	if((shmid = shmget(SHMKEY, SHMSIZE, IPC_CREAT|0666)) == -1){
		perror("shmget error");
		exit(1);
	}
	
	if((shmaddr = shmat(shmid, NULL, 0)) == (void *)-1){
		perror("shmat error");
		exit(1);
	}
	
	printf("공유메모리를 통해 받은 문자열 : %s", (char *)shmaddr);
	
	if(shmdt(shmaddr) == -1){
		perror("shmdt error");
		exit(1);
	}
	
	if(shmctl(shmid, IPC_RMID, 0) == -1){
		perror("shmctl error");
		exit(1);
	}
}