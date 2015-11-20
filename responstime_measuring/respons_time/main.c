#include "network.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define UDP_PORT 9999
#define SERVER_IP "192.168.0.1"

typedef struct{
	float Kp;
	float Ki;
	float Kd;
	float ref;
	float period;
}pid_const_t;

typedef struct{
	float y;
	float u;
	float integral;
	float err;
	float prev_err;
	float period;

}pid_var_t;

static float buftof(char *buf, int res);
static float pid(pid_var_t *var, pid_const_t PID);

int main(){
	static char buf[20];
	static FILE *fp;
	fp =  fopen ((const char*) "resp_onlyset.csv", (const char*) "w");
	fputs((const char*) "\n", fp);
	fputs((const char*) "Start_ns, stop_ns,  spent_ns\n", fp);
	
	struct timespec timer;
	struct timespec timer2;
	struct udp_conn udp_socket;
	udp_init_client(&udp_socket, UDP_PORT, SERVER_IP);
	send_start(&udp_socket);
	for(int i = 0; i < 10000;i++){
		clock_gettime(CLOCK_REALTIME  , &timer);
		get_data(&udp_socket);
		while_recive(&udp_socket, buf, sizeof(buf));
		set_float(&udp_socket, 0.0);
		clock_gettime(CLOCK_REALTIME  , &timer2);
		fprintf(fp,"%li, %li, %li\n",timer.tv_nsec, timer2.tv_nsec, timer2.tv_nsec-timer.tv_nsec);
	}
	send_stop(&udp_socket);
	udp_close(&udp_socket);
	fclose(fp);
	return 0;
}