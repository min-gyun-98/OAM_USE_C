### 1. 공유 메모리<br>
- 프로세스에서 메모리는 해당 프로세스만이 사용하는게 일반적이다.
- 메모리에는 명령어, 지역 변수, 동적 변수, 전역 변수와 같이 데이터가 존재하는데 해당 프로세스만이 접근하고 변경 가능하다. 
- 이러한 메모리를 다른 프로세스에서 사용 할 수 있도록 만들수 있는데 그 방법이 공유 메모리라는 IPC 기법
### 관련 함수
- 공유 메모리 사용을 위해서는 관련 헤더파일이 필요<br>
```c
#include <sys/shm.h>
#include <sys/ipc.h>
```
1. shmget : 인자로 전달된 key의 값으로 공유 메모리를 얻고 사용 하려는 공유 메모리 값을 전달
```c
int shmget(key_t key, size_t size, int shmflg);
```
2. shmat : 얻은 공유 메모리를 프로세스에 연결시켜주는 함수, 연결 성공 시 포인터 값을 넘겨주고 포인터에 원하는 데이터를 넣어 데이터를 공유 할 수 있다.
```c
void *shmat(int shmid, const void * shmaddr, int shmflg);
```
3. shmdt : 공유 메모리를 프로세스와 분리시키는 함수, shmat에서 받은 포인터를 전달
```c
int shmdt(const void *shmaddr);
```
4. shmctl : 공유 메모리를 제어 하기 위한 함수
```c
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```
### 공유 메모리를 사용 데이터 주고 받기 결과
![image](https://user-images.githubusercontent.com/82010586/125216377-fe3b5480-e2f8-11eb-9bce-eedb26d8b04b.png)

### 2. 메시지 큐
- IPC기법 중 하나인 메시지 큐는 Data Structrue 중 큐를 사용
- 기본적으로 먼저 들어온 메시지가 먼저 나가는 방식
- 메시지 큐에 들어온 메시지는 커널어서 보관하고 있기 때문에 프로세스가 종료되어도 사라지지 않음
- 메시지 큐의 용량이 허용하는 한 메시지는 계속 큐에 쌓이게 됨
- 메시지 큐에서 메시지를 얻어오는 쪽은 읽은 메시지를 메시지 큐에서 삭제함

### 관련 함수
- 메시지 큐 사용을 위해 필요한 헤더 파일
```c
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
```
1. msgget : 고유 key 값을 넣어 메시지 큐 id를 얻어오는 함수, flag는 IPC_CREAT, IPC_EXCL를 지정 할 수 있음
```c
int msgget(key_t key, int msgflg);
```
2. msgsnd : 메시지 큐에 메시지를 보내는 함수, msgget에서 얻어온 id 값을 넣어 메시지 큐를 지정하고 메시지를 보냄
```c
int msgsnd(int msqid, struct msgbuf *msgp, size_t msgsz, int msgflg);
```
- msggp : msggp는 구조체로 아래와 같이 정의 해야함
```c
struct msgbuf{
  long mtype;
  char mtext[1];
};
// mtype은 메시지 타입을 나타내는데 값은 0보다 커야함
// mtext는 실제 메시지 큐에 보낼 데이터를 지정함
```
3. msgrcv : 메시지를 받는 함수, 지정한 메시지 큐 id의 메시지를 하나 읽고 읽은 메세지를 메시지 큐에서 제거
```c
ssize_t msgrcv(int msqid, struct msgbuf *msgp, size_t msgsz, long msgtyp, int msgflg);
// 읽어온 메시지를 msgp에 mtext 부분에 저장한다고 생각하면됨 
```
- msgtyp으로 읽어올 메시지 타입을 지정 할 수 있음   
- 0일 경우 : 메시지 큐의 첫 번째 메시지를 읽어옴
- 0보다 클 경우 : 지정한 숫자값과 동일한 메시지 타입을 갖는 메시지를 읽어옴
- 0보다 작은 경우 : msgtyp의 절대값 이하의 가장 작은 메시지를 읽어옴
4. msgctl : 메시지 큐를 제어하기 위한 함수
```c
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```
### 메시지 큐 사용 데이터 주고 받기 결과
![메시지 큐 실습 결과](https://user-images.githubusercontent.com/82010586/125218977-fed6e980-e2fe-11eb-9b15-f47674c0f14a.PNG)


### 3. 소켓
- 소프트웨어로 작성된 추상적인 개념의 통신 접속점이라고 할 수 있는데 네트워크 응용 프로그램은 소켓을 통하여 통신망으로 데이터를 송수신하게 됨
- 응용프로그램에서 TCP/IP를 이용하는 창구 역할을 하며 응용 프로그램과 소켓 사이의 인터페이스 역할을 수행
- 두 프로그램의 두 소켓이 연결되면 다른 프로세스끼리 데이터를 전달 하는 것이 가능

### 관련 함수
- 소켓의 경우 서버 소켓, 클라이언트 소켓 사용 함수가 다름
![소켓 실행 순서](https://user-images.githubusercontent.com/82010586/125220872-490d9a00-e302-11eb-9fe8-d321f630d42e.PNG)

- 소켓 사용 시 필요한 헤더 파일
```c
#include <sys/socket.h>
```
- 서버 소켓 사용 함수
1. socket() : 소켓을 생성하는 함수
```c
socket(int domain, int type, int protocol);
```
2. bind() : 소켓과 서버의 정보를 묶어주는 함수
```c
bind(int fd, struct sockaddr *local_addr, socklen_t addr_length);
```
3. listen() : 어떤 컴퓨터로부터 요청이 와도 수락할 수 있게 대기 상태에 들어가는 함수
```c
listen(int fd, int backlog_queue_size);
```
4. accept() : 서버 소켓에다가 클라이언트를 연결하는 함수
```c
accept(int fd, sockaddr *remote_host, socklen_t *addr_length);
```
5. read() : 데이터를 받는 함수
```c
read(int fd, const void* buf, int length);
```
6. write() : 데이터를 보내는 함수
```c
write(int fd, const void *buf, int length);
```
7. close(): 소켓을 닫고 통신을 종료하는 함수
```c
close(int fd);
```
- 클라이언트 소켓 사용 함수
1. socket() : 소켓을 생성하는 함수
```c
socket(int domain, int type, int protocol);
```
2. connect() : 서버로 연결을 요청하는 함수
```c
connect(int fd, struct sockaddr* serv_addr, socklen_t addrlen);
```
3. read() : 데이터를 받는 함수
```c
read(int fd, const void* buf, int length);
```
4. write() : 데이터를 보내는 함수
```c
write(int fd, const void *buf, int length);
```
5. close() : 소켓을 닫고 통신을 종료하는 함수
```c
close(int fd);
```
### 소켓 사용 데이터 주고 받기 결과
![소켓 실습 결과1](https://user-images.githubusercontent.com/82010586/125223412-af94b700-e306-11eb-91b3-225dd47e39e6.PNG)
![소켓 실습 결과2](https://user-images.githubusercontent.com/82010586/125223413-b02d4d80-e306-11eb-882f-a725f35b8d7e.PNG)

