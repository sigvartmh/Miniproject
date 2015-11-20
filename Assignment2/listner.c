#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#include "global.h"
#include "miniproject.h"
#include "network.h"

extern update_t update;
static float buftof(char *buf, int res);


void* UDPlistner() {
	struct timespec delay;
	clock_gettime(CLOCK_REALTIME, &delay);
	//printf("Started listner\r\n");
	char buf[20] = {0};
	int res = 0;
	sem_post(&listnerstarted);
	while(1){	
		res = while_recive(&udp_socket, buf, sizeof(buf));
		if(buf[0] == 'S'){
			sem_post(&signal);
		}else if(buf[0] == 'G'){
			sem_post(&update.signal);
			pthread_mutex_lock(&update.lock);
			update.y = buftof(buf, res);
			pthread_mutex_unlock(&update.lock);
		}
	}
	return NULL;

}

static float buftof(char *buf, int res){
	char tmp[20]; //This wastes a bit of memory but who cares
	memcpy(tmp, &buf[8], res-7);
	float converted = atof(tmp);
	return converted;
}
