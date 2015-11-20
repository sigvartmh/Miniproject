#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#include "network.h"
#include "global.h"

#ifdef SIGNAL

void* signalHandler(){
	while(1){
		sem_wait(&signal);
		if( send_signal(&udp_socket) == -1){
			perror("Error sending signal\n");
		}
	}
	return NULL;
}
#endif
