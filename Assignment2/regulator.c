#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#include "network.h"
#include "miniproject.h"
#include "global.h"

#define US 1000000
#define NS 1000000000
#define PERIOD 0.0040
#define PERIOD_NS PERIOD*NS
#define PERIOD_US PERIOD*US
#define RUNTIME_NS 500000000

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

static float pid(pid_var_t *var, pid_const_t PID);

void* PIDthread(){
	sem_wait(&listnerstarted);

	pid_const_t PID = {
		.Kp = 10,
		.Ki = 800,
		.Kd = 1.0,
		.ref = 1.0,
		.period = PERIOD,
	};

	pid_var_t var = {
		.y=0,
		.u=0,
		.integral = 0.0,
		.err = -2.0,
	};
	
	/*
	if (pthread_mutex_init(&var.lock, NULL) != 0){
		perror("Failed initializing PID mutex");
	}*/

	struct timespec delay;
	clock_gettime(CLOCK_REALTIME, &delay);

	int counter = 0;
	while(counter*PERIOD_NS < RUNTIME_NS){
		get_data(&udp_socket);
		sem_wait(&update.signal);
		pthread_mutex_lock(&update.lock);
		var.y = update.y;
		pthread_mutex_unlock(&update.lock);

		var.u = pid(&var, PID);
		set_float(&udp_socket, var.u); // error checking 
		counter++;
		timespec_add_us(&delay, PERIOD_US);
		clock_nanosleep(&delay);	 				

	}
	//printf("Finished regulator");
	return NULL;
}

static float pid(pid_var_t *var, pid_const_t PID){
		
	float prev_err = 0;
	float derivative = 0;
	float u = 0; // return value

	prev_err = var->err; 
	var->err = PID.ref - var->y;
		
	derivative = (var->err - prev_err);
	var->integral = var->integral + (var->err * PID.period);
		
	u = PID.Kp*var->err + PID.Ki*var->integral + PID.Kd*derivative;
		
	return u;
}




