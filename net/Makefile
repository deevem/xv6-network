%.o : %.c
	gcc -c $^ -Wall

testudp: testudp.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o 
	gcc -o testudp testudp.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o -Wall
clean: 
	-rm -f *.o