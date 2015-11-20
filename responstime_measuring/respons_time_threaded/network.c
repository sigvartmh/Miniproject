#include <pthread.h>
#include <stdio.h>

#include "miniproject.h"
#include "network.h"

/* Threadsafe network helper functions */

int send_stop(struct udp_conn *udp){
	char buf[]="STOP\0";
	pthread_mutex_lock(&udp->lock);
	int send = udp_send(udp , buf,sizeof(buf));
	if(send == -1) perror("Failure in sending stop");
	pthread_mutex_unlock(&udp->lock);
	return send;
}

int send_start(struct udp_conn *udp){
	//printf("Insnding Start\r\n");
	char buf[]="START\0";
	pthread_mutex_lock(&udp->lock);
	int send = udp_send(udp , buf,sizeof(buf));
	if(send == -1) perror("Failure in sending stop");
	pthread_mutex_unlock(&udp->lock);
	return send;
}

int set_float(struct udp_conn *udp, float f){
	char buf[20];
	sprintf(buf, "SET:%f", f);
	
	pthread_mutex_lock(&udp->lock);
	int send = udp_send(udp,buf,sizeof(buf));
	pthread_mutex_unlock(&udp->lock);
	
	return send;
}

int send_signal(struct udp_conn *udp){
	char buf[] = "SIGNAL_ACK\0";
	
	pthread_mutex_lock(&udp->lock);
	int send = udp_send(udp,buf,sizeof(buf));
	pthread_mutex_unlock(&udp->lock);
	
	return send;
}
int get_data(struct udp_conn *udp){
	//printf("Insnding GET\r\n");
	char buf[] = "GET\0";
	pthread_mutex_lock(&udp->lock);
	//printf("Sending get\r\n");
	int send = udp_send(udp,buf,sizeof(buf));
	//printf("Sent get\r\n");
	pthread_mutex_unlock(&udp->lock);
	return send;
}


int recive(struct udp_conn *udp, char *buf, int len){
	int res = 0;
	//pthread_mutex_lock(&udp->lock);
	res = udp_receive(udp, buf,len);
	//pthread_mutex_unlock(&udp->lock);

	
	return res;
}
