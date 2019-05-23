/*
 * commServer.h
 *
 *  Created on: May 13, 2019
 *      Author: sangbeom.lim@colubrismx.com
 */

#ifndef INC_COMMSERVER_H_
#define INC_COMMSERVER_H_



//include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <error.h>
#include <pthread.h>

//define
#define IP "127.0.0.1"
#define PORT 9000
#define MAX_BUF 1024
#define CLIENT 1 //client number
#define MAXSLEEP 5


//global value

//func
void *logClientProc(void *data);
void senddata(int sock);
void disconnect(int sock);
int reconnect(int sock, const struct sockaddr *addr, socklen_t length);
int createsocket(int sock, struct sockaddr_in addr, int port, char *ip);




#endif /* INC_COMMSERVER_H_ */
