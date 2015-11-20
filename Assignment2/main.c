#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "network.h"

#include "regulator.h"
#include "listner.h"
#include "signal.h"

typedef struct{
	float y;
	sem_t signal;
	pthread_mutex_t lock;
}update_t;

sem_t signal;
sem_t listnerstarted;
update_t update;
struct udp_conn udp_socket;

#define UDP_PORT 9999
#define SERVER_IP "192.168.0.1"


int main() {
	
	int err = udp_init_client(&udp_socket, UDP_PORT, SERVER_IP);
	
	sem_init(&signal, 0,0);
	sem_init(&update.signal, 0,0);
	sem_init(&listnerstarted, 0,0);
	if (pthread_mutex_init(&update.lock, NULL) != 0){
		perror("Error intialzing update mutex");
	}

	if (err!=0){
		perror("Error connecting to server");
	}

	pthread_t listner,
			  regulator,
			  sig_handler;
			  
	
	send_start(&udp_socket);
	
	err = pthread_create(&listner, NULL, UDPlistner, NULL);
	if (err!=0)
	{
		perror("Thread Creation failed");
	}

	err = pthread_create(&regulator, NULL, PIDthread, NULL);
	if (err!=0)
	{
		perror("Thread Creation failed");
	}
	#ifdef SIGNAL
	err = pthread_create(&sig_handler, NULL, signalHandler,NULL );
	if (err!=0)
	{
		perror("Thread Creation failed");
	}
	#endif


	pthread_join(regulator, NULL);

	
	send_stop(&udp_socket);
	
	sem_destroy(&signal);
	sem_destroy(&update.signal);
	pthread_mutex_destroy(&udp_socket.lock);
	
	
	return 0;
	
}

/* Clean up mutex/semaphores */

