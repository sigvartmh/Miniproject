#Miniproject

### Assignment 1:

Design choices:

1st we tried to find the period of the controller from doing a 10000 sample with get, recive and set commands and log the time to a .csv file on NFS of the AVR32. This however proved to be quite inaccurate. 

Instead we took the data produced by the example server in the pid_time.dat from the 10000 samples and looked at the time between get respones and set respones time. This provided us with the data to say that the maximum delay time would be around 1700-1900 us so we rounded it up to 2000 as the clock_nanosleep proved to be quite inaccurate.

We decided to use a PID controller with a KD factor of 1 and since the noise from the system were constantly 3 and the reference we set were 1.  We sat the initial error to be -2.0. We could  have tried to measure the time  it took for the pid calculation and removed it from the delay but we decided that the response of the system was allready good enough.

![/assignment1/plot4_10000samples.png](plot/assignment1/plot4_10000samples.png)

<img src="https://github.com/sigvartmh/Miniproject/blob/master/plot/assaignment1/plot4_10000samples.png" alt="10000samples assignment1" > 

As you can see from the plots the maximum time it takes for a get respons is around 2000 us.

![plot/assignment1/plot4.png](plot/assignment1/plot4.png)

Here you can see that none of the deadlines are missed as the controller step time from `GET` is always lower or on  pair with the period time between the `GET` responses. 


### Assignment 2:

Design choices:

####Threads
We went for having 4 threads where the 4th thread is just a thread which initializes and joins the threads, cleans up and initializes global variables.

The 3 other threads are:
	1. Udp listner
	2. PID regulator
	3. Signal responder


####Semaphores
Between these threads we used semaphores to syncronize and signal the threads. One for waiting for the listner thread to be started before trying to GET in the regulator thread. 

Two other semaphores were used to signal threads that data was ready. `update.signal` signalied the regulator that a new `GET_ACK` was recived and that new output data was ready. 

`signal` was used to tell the signal responder thread that a signal was recived from the server so it would respond with `SIGNAL_ACK`.

####Mutexes
The UDP functions were reimplemented with helper functions to make them easier to use and also thread safe. As we added a mutex to the `struct udp_conn` in the `miniproject.h` and initializer in `udp_init_client`. All functions were protected by mutexes except the recive function. This was to make sure that multiple threads did not try to send at the  same  time.

Specified by the miniproject should only used by 1 thread as it is doing the reception UDP packets and not sending. So recive will not create any conflicts/side effects on the other threads doing the sending.

Also the conversion from string to float is done in the listner and the data structure is protected by the `update.lock` mutex. We found that it had the best preformance  when we
did the conversion in  the listner rather than the regulator. Probably due to less task
switching overhead and reduced the use of memcpy on the buffer.

####Effect of signal on regulator
Without signal:
![(plot5_nosignal.png)l](plot5_nosignal.png)


With signal:
![(plot5_withsignal.png)](plot5_withsignal.png)

As you can see the effect of the signal on the regulator is controler step time goes over the period set for the get request period. This is due to the PID task or other task being  interupted so that the signaling task is using some of the cpu time making it so the PID does not reach its deadline. As the GET period gets more spread.

Therfor to get a response we had to increase the period to 4000us which we found using the same method as in Assignment 1 where we took 10000 samples and found the max from the pid_data.dat.

As  you can see from this plot no deadlines are missed as the period time is always higher or  on pair  with the controller step time. This results is not always stable, due to linux system task running in the background both on the server and  hostcard. Therefor the conclusion is that the program if  possible should be run with system priority and with a RT hardened kernel or RT API like Xiaomi.  

When error occurs see plot5_err.png


TL;DR:
* Semaphores for signaling events
* Mutexes for protecting globalvariables/datastructures
* Delay in UPDlistner for freeing up CPU time(removed)
* Period found with same method as in  Assignment 1.
* No Deadlines missed
* Few oscillations and small overshoot but more than Assignment 1. 