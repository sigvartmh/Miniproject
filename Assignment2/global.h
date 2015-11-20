#ifndef GLOBAL_H_
#define GLOBAL_H_

#include<semaphore.h>
#include<pthread.h>

typedef struct{
	float y;
	sem_t signal;
	pthread_mutex_t lock;
}update_t;

extern update_t update;
extern sem_t signal;
extern struct udp_conn udp_socket;
extern sem_t listnerstarted;

#endif /* GLOBAL_H_ */
