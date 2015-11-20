#include <pthread.h>
#include <stdio.h>

#include "miniproject.h"
#include "network.h"
#include "global.h"

/* Thread safe network helper functions */

int send_stop(struct udp_conn *udp){
	char buf[]="STOP\0";
	pthread_mutex_lock(&udp->lock);
	int send = udp_send(udp , buf,sizeof(buf));
	pthread_mutex_unlock(&udp->lock);
	if(send == -1) perror("Failure in sending stop");
	return send;
}

int send_start(struct udp_conn *udp){
	//printf("Insnding Start\r\n");
	char buf[]="START\0";
	pthread_mutex_lock(&udp->lock);
	int send = udp_send(udp , buf,sizeof(buf));
	pthread_mutex_unlock(&udp->lock);
	if(send == -1) perror("Failure in sending stop");
	return send;
}

int set_float(struct udp_conn *udp, float f){
	char buf[20];
	sprintf(buf, "SET:%f", f);
	
	pthread_mutex_lock(&udp->lock);
	int send = udp_send(udp,buf,sizeof(buf));
	pthread_mutex_unlock(&udp->lock);
	if(send == -1) perror("Failure in sending float");
	return send;
}

int send_signal(struct udp_conn *udp){
	char buf[] = "SIGNAL_ACK\0";
	
	pthread_mutex_lock(&udp->lock);
	int send = udp_send(udp,buf,sizeof(buf));
	pthread_mutex_unlock(&udp->lock);
	if(send == -1) perror("Failure in sending signal");
	return send;
}
int get_data(struct udp_conn *udp){
	char buf[] = "GET\0";
	pthread_mutex_lock(&udp->lock);
	int send = udp_send(udp,buf,sizeof(buf));
	pthread_mutex_unlock(&udp->lock);
	if(send == -1) perror("Failure in sending GET");
	return send;
}

/* Recive could not be made threadsafe due to its spinlock nature */
int while_recive(struct udp_conn *udp, char *buf, int len){
	int res = 0;
	//pthread_mutex_lock(&udp->lock);
	res = udp_receive(udp, buf,len);
	//pthread_mutex_unlock(&udp->lock);

	
	return res;
}
