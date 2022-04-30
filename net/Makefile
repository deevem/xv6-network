%.o : %.c
	gcc -c $^ -Wall

testudp: testudp.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o 
	gcc -o testudp testudp.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o -Wall

recvudp: recvudp.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o 
	gcc -o recvudp recvudp.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o -Wall

icmptest: icmptest.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o icmp.o 
	gcc -o icmptest icmptest.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o icmp.o -Wall

clean: 
	-rm -f *.o
	-rm -f recvudp testudp