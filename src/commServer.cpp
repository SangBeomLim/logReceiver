/*
 * commServer.cpp
 *
 *  Created on: May 13, 2019
 *      Author: sangbeom.lim@colubrismx.com
 */
#include "../inc/commServer.h"

//send data
void senddata(int sock)
{
	char buf[MAX_BUF];

	memset(buf, 0x00, sizeof(MAX_BUF));
	sprintf(buf, "TCP/IP SOCKET PROGRAMMING", sizeof(MAX_BUF));
	while(1)
	{
		if(write(sock, (char*)&buf, sizeof(buf)) <= 0)// send to server
		{
			perror("write error ");
		}
		printf("client : %s\n", buf);

		sleep(1);
	}
//	read(sock, &buf, sizeof(buf));
//	printf("server value : %d\n",buf);
}
//disconnecting
void disconnect(int sock)
{
	printf("disconnect server & close socket\n");
	close(sock);
}

//reconnecting
int reconnect(int sock, const struct sockaddr *addr, socklen_t length)
{
	int msec;

	//try to create socket
	//try to connect
	for(msec = 1; msec <= MAXSLEEP ; msec <<= 1)
	{
		if(connect(sock, (struct sockaddr *)&addr, length) == 0)
		{

			printf("reconnecting...\n");
			senddata(sock);
			return 0;
		}

		if(msec <= MAXSLEEP/2)
			sleep(msec);
	}



	return -1;
}


int createsocket(int sock, struct sockaddr_in addr, int port, char *ip)
{


	if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket fail");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);
	//connecting
	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("connect fail ");
		goto FAILED;
	}

	senddata(sock);
		//goto Failed
FAILED:
//	reconnect(sock, addr, sizeof(addr));

	return 0;
}

void *logClientProc(void *data)
{
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	char *thread_name = (char*)data;
	struct sockaddr_in servaddr;
	int sockfd = 0;


	//tcp/ip client
	createsocket(sockfd, servaddr, PORT, IP);
	while(1)
	{
		printf("[%s] pid:%u, tid:%x\n",thread_name, (unsigned int)pid, (unsigned int)tid);
		sleep(10);
	}




}
