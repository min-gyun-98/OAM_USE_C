# 현장실습
- 각각 실습 코드를 다른 폴더에 저장
- putty로 연결하여 코드를 컴파일하고 실행
## 실습1. 기본적인 IPC 통신 3가지를 이용하여 메시지 송신, 수신
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
### 공유 메모리를 사용 데이터 송신, 수신 결과
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
### 메시지 큐 사용 데이터 송신, 수신 결과
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
### 소켓 사용 데이터 송신, 수신 결과
![소켓 실습 결과1](https://user-images.githubusercontent.com/82010586/125223412-af94b700-e306-11eb-91b3-225dd47e39e6.PNG)
![소켓 실습 결과2](https://user-images.githubusercontent.com/82010586/125223413-b02d4d80-e306-11eb-882f-a725f35b8d7e.PNG)

## 실습2. 클라이언트 프로그램에서 특정 명령어를 서버 프로그램에 전송하여 명령어에 맞는 코드 수행 후 결과 값을 클라이언트로 전송하는 프로그램(소켓 사용)
- 실습2의 경우 소켓을 이용하여 명령어 전달 및 명령어 수행 결과 값 전달을 실습함
- UDP 방식과 TCP 방식 2가지를 이용하여 프로그램을 작성
- 명령어는 list, write, show 3가지 중 하나를 선택해서 사용
- list : 게시글 목록을 클라이언트에 전달
- write (제목) (내용) : 서버에 (글번호).txt 파일을 생성하고 파일에 제목, 내용이 저장
- show (글번호) : 글번호의 맞는 파일을 찾고 파일의 제목과 내용을 전달

### 프로그램 흐름
![실습2 실행 순서](https://user-images.githubusercontent.com/82010586/125226632-66476600-e30c-11eb-90cf-85e68efa7f77.PNG)

### UDP와 TCP
- 소켓은 기본적으로 2가지 통신 방법을 가지고 있음. 신뢰성 프로토콜인 TCP와 비신뢰성 프로토콜인 UDP
### 1. TCP
- TCP 통신은 전화와 같은 방식으로 작동. 상대방의 전화번호를 알고 있어야 전화를 거는 것처럼 TCP 통신을 하기 위해서는 IP주소와 Port를 알고 있어야 연결을 요청할 수 있음
- 서버는 응답을 받기위해서 계속 대기
- 마찬가지로 서버가 응답을 받아주지 않으면 계속적으로 요청
- 서로 연결이 된 경우 양방향으로 통신이 가능. 계속적으로 데이터를 주고 받을 수 있음
- 데이터를 잘 주고 받았는지 확인 절차도 거치게 됨
- UDP에 비해서 속도가 느림
- 연결지향성, 신뢰성 있음, 재전송 요청함(오류 및 패킷 손실 검출시)

### 2. UDP
- UDP 통신의 경우 상대방과 연결되어 있다는 개념이 아님. 편지와 비슷하다고 생각하면 됨
- 단순히 보내려는 주소와 포트를 알고 데이터를 전달하는 방식
- 보냈다는 사실은 존재하지만 상대방이 데이터를 잘 받았는지는 확인할 방법이 없음
- 본인 로컬 머신에 소켓을 만들고 직접 데이터가 들어왔는지 확인해야함
- 단순 데이터를 보내고 받고만 하기 때문에 TCP보다 속도가 빠름
- 비연결성, 신뢰성이 없음, 재전송 없음, 신뢰성을 보장하지않지만 고속 데이터 전송이 가능하여 실시간 전송에 적합

### 코드 실행 결과(UDP, TCP 코드는 다르지만 실행 결과는 같음)
![실습2 실습 결과1](https://user-images.githubusercontent.com/82010586/125236450-f393b600-e31e-11eb-9207-7ed8aaa22e58.PNG)
![실습2 실습 결과2](https://user-images.githubusercontent.com/82010586/125236455-f4c4e300-e31e-11eb-987c-43f1ee5577a2.PNG)

## 실습3. EMS + emsd, agtd, 서비스(호처리) 프로그램 실습 (소켓, 메시지 큐 사용)
- mmi(EMS + emsd) : 명령을 입력해서 전송하고, 명령을 수행한 결과 값을 출력해주는 역할(실습2의 client와 유사)
- agtd : mmi의 서버가 되는 프로그램. 서비스 프로그램으로 명령어를 전달, 서비스 프로그램 결과값을 받아 mmi로 전송
- 서비스(호처리) 프로그램 : 명령을 전송 받으면, 해당 명령을 처리하여 agtd에 전달해주는 프로그램(실습2의 server와 유사)
- rstat : 리소스 상태 확인 명령어 수행 프로그램
- prm : 프로세스 상태 확인 명령어 수행 프로그램
- 실행 가능한 명령어 목록
1. 리소스 상태 확인 기능
- 명령어 : DIS-RESOURCE
- 인자 : MEMORY(메모리 사용률 확인), DISK(디스크 사용률 확인), CPU(CPU 사용률 확인)
2. 프로세스 상태 확인 기능
- 명령어 : DIS-SW-STS
- 인자 : ACT(rstat 프로세스 상태를 확인)

### 프로그램 흐름
![실습3 실행 순서](https://user-images.githubusercontent.com/82010586/125246566-e8478700-e32c-11eb-83c8-bcf481b03753.PNG)

### 코드 실행 결과(rstat, prm은 실행중인 상태)
![실습3 실행 결과1](https://user-images.githubusercontent.com/82010586/125254709-aec74980-e335-11eb-99d9-19be5045e3f9.PNG)
![실습3 실행 결과2](https://user-images.githubusercontent.com/82010586/125254716-af5fe000-e335-11eb-9929-cf86d894b46b.PNG)

