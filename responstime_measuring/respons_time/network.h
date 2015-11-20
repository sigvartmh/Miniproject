#ifndef NETWORK_H_
#define NETWORK_H_

#include "miniproject.h"

int get_data(struct udp_conn *udp);
int send_start(struct udp_conn *udp);
int send_signal(struct udp_conn *udp);
int send_stop(struct udp_conn *udp);
int set_float(struct udp_conn *udp, float f);
int while_recive(struct udp_conn *udp, char *buf, int len);

#endif /* NETWORK_H_ */
