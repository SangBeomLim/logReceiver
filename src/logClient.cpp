/*
 * logClient.cpp
 *
 *  Created on: May 9, 2019
 *      Author: sangbeom.lim@colubrismx.com
 *
 *
 */
#include "../inc/logClient.h"
#include "../inc/commServer.h"


struct mq_attr initAttribute()
{
	struct mq_attr _attr;
	//First set up the attribute structure
	_attr.mq_maxmsg = 20;
	_attr.mq_msgsize = MSG_SIZE; //1024
	_attr.mq_flags = 0; // BLOCK or NON-BLOCK

	return _attr;
}

mqd_t onOpenlogClient(struct mq_attr attr)
{
	mqd_t mq;
	//open msg queue
	mq = mq_open(QUEUE, O_RDWR | O_CREAT, 0666, &attr);

	return mq;
}

void onRecvlogClient(mqd_t mq, struct mq_attr attr, DATA data)
{
	//set Receive data
	while(1)
	{
		memset(&data, 0x00, sizeof(DATA));
		if((mq_receive(mq, (char*)&data, attr.mq_msgsize, NULL)) == -1)
		{
			perror("recv failed ");
			exit(0);
		}

		printf( "recv %d type : %d text : %s \n", data.seq, data.loglv, data.msgdata);

		fflush(stdout);
		usleep(10);
	}

	return;
}

void onCloselogClient(mqd_t mq)
{
	// This will now be a temporary queue...as soon as it's closed,
	// it will be removed
	mq_unlink(QUEUE);
	//Close all open message queue descriptor
	mq_close(mq);
}

void *logManagerProc(void *arg)
{
	DATA data;
	mqd_t mqdes;
	struct mq_attr mqattr; //to store queue attributes
//	struct sigevent sigevent; //for notification

	mqattr = initAttribute();
	mqdes = onOpenlogClient(mqattr);
	if(mqdes == -1)
	{
		perror("open error ");
		exit(0);
	}
	onRecvlogClient(mqdes, mqattr, data);
	onCloselogClient(mqdes);
	return NULL;
}

void start_thread(int num, char *tname)
{
	// 쓰레드 시작
	thread_exit = 0; //종료 온오프
	if(num == 0)
		thread_id = pthread_create(&m_thread[num], NULL, logManagerProc, (void*)tname);
	else if(num == 1)
		thread_id = pthread_create(&m_thread[num], NULL, logClientProc, (void*)tname);

	return;
}

void exit_thread(int num)
{
	// 쓰레드 종료
	thread_exit = 1; //종료 온오프
	pthread_join(m_thread[num], NULL);

	return;
}
int main(int argc, char **argv)
{

	char name1[] = "logManager"; //쓰레드 이름
	char name2[] = "logClient";

	start_thread(0, name1); //쓰레드 시작
	start_thread(1, name2);

	exit_thread(0); //쓰레드 종료
	exit_thread(1);
	return 0;
}

