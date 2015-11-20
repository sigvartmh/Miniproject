#include "network.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

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

sem_t update;
sem_t listner_started;
struct udp_conn udp_socket;

//static float buftof(char *buf, int res);
//static float pid(pid_var_t *var, pid_const_t PID);
#define TIME_SIGNAL

void* reg(){
	struct timespec timer;
	struct timespec timer2;
	#ifdef WRITE
	static FILE *fp;
	fp =  fopen ((const char*) "resp_signal_and_set_threaded.csv", (const char*) "w");
	fputs((const char*) "\n", fp);
	fputs((const char*) "Start_ns, stop_ns,  spent_ns\n", fp);
	#endif
	sem_wait(&listner_started);
	for(int i = 0; i < 10000;i++){
		clock_gettime(CLOCK_REALTIME  , &timer);
		get_data(&udp_socket);
		sem_wait(&update);
		set_float(&udp_socket, 0.0);
		clock_gettime(CLOCK_REALTIME  , &timer2);
		#ifdef WRITE
		fprintf(fp,"%li, %li, %li\n",timer.tv_nsec, timer2.tv_nsec, timer2.tv_nsec-timer.tv_nsec);
		#endif
	}
	fclose(fp);
}

void* UDPlistner(){
	static char buf[20];
	sem_post(&listner_started);
	while(1){
		recive(&udp_socket, buf, sizeof(buf));
		#ifdef TIME_SIGNAL
		if (buf[0] == 'S')
		{
			send_signal(&udp_socket);
			/* code */
		}
		#endif
		sem_post(&update);
	}
}

int main(){
	
	pthread_t listner, regulator;

	sem_init(&update, 0,0);
	sem_init(&listner_started, 0,0);
	int err = 0;

	

	udp_init_client(&udp_socket, UDP_PORT, SERVER_IP);
	send_start(&udp_socket);
	err = pthread_create(&listner, NULL, UDPlistner, NULL);
	if (err!=0)
	{
		perror("Thread Creation failed");
	}

	err = pthread_create(&regulator, NULL, reg, NULL);
	if (err!=0)
	{
		perror("Thread Creation failed");
	}
	pthread_join(regulator, NULL);
	send_stop(&udp_socket);
	udp_close(&udp_socket);
	
	return 0;
}

