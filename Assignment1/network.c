#include <stdio.h>
#include "miniproject.h"
#include "network.h"

/* Thread safe network helper functions */

int send_stop(struct udp_conn *udp){
	char buf[]="STOP\0";
	
	int send = udp_send(udp , buf,sizeof(buf));	
	return send;
}

int send_start(struct udp_conn *udp){
	//printf("Insnding Start\r\n");
	char buf[]="START\0";
	
	int send = udp_send(udp , buf,sizeof(buf));
	
	return send;
}

int set_float(struct udp_conn *udp, float f){
	char buf[20];
	sprintf(buf, "SET:%f", f);
	
	
	int send = udp_send(udp,buf,sizeof(buf));
	
	
	return send;
}

int send_signal(struct udp_conn *udp){
	char buf[] = "SIGNAL_ACK\0";
	
	
	int send = udp_send(udp,buf,sizeof(buf));
	
	
	return send;
}
int get_data(struct udp_conn *udp){
	//printf("Insnding GET\r\n");
	char buf[] = "GET\0";
	
	//printf("Sending get\r\n");
	int send = udp_send(udp,buf,sizeof(buf));
	//printf("Sent get\r\n");
	
	return send;
}


int while_recive(struct udp_conn *udp, char *buf, int len){
	int res = 0;
	//
	res = udp_receive(udp, buf,len);
	//

	
	return res;
}
