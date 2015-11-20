#include "network.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define UDP_PORT 9999
#define SERVER_IP "192.168.0.1"



clock_t begin, end;
float time_spent;

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

//#define PERIOD 0.0045
//#define PERIOD 0.0000210000
#define PERIOD 0.0002500000
//#define PERIOD_NS 2100000
#define PERIOD_NS 2000000
#define RUNTIME_NS 600000000
#define PERIOD_US 2000
//#define PID_PER 0.0024


int main(){
	char buf[20]  = {0};
	struct timespec delay;
	struct udp_conn udp_socket;

	pid_const_t PID = {
		.Kp = 11,
		.Ki = 850,
		.Kd = 1.0,
		.ref = 1.0,
		.period = 0.002,
	};

	pid_var_t var = {
		.y=0,
		.u=0,
		.integral = 0.0,
		.err = -2.0,
	};
	int res=0;
	float out;
	udp_init_client(&udp_socket, UDP_PORT, SERVER_IP);
	send_start(&udp_socket);
	
	int i = 0;
	clock_gettime(CLOCK_REALTIME  , &delay);
	
	while(PERIOD_NS*i < RUNTIME_NS){
		get_data(&udp_socket);
		res = while_recive(&udp_socket, buf, sizeof(buf));
		if(buf[0]=='G'){
			var.y = buftof(buf,res);
			out = pid(&var, PID);
			set_float(&udp_socket, out);
		}
		i++;
		timespec_add_us(&delay, PERIOD_US);
		clock_nanosleep(&delay);
	}
	
	send_stop(&udp_socket);
	udp_close(&udp_socket);
	return 0;
}


static float buftof(char *buf, int res){
	char tmp[20];
	memcpy(tmp, &buf[8], res-7);
	float converted = atof(tmp);
	return converted;
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

