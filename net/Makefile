%.o : %.c
	gcc -c $^ -Wall

testudp: testudp.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o icmp.o dns.o 
	gcc -o testudp testudp.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o icmp.o dns.o -Wall

recvudp: recvudp.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o icmp.o dns.o 
	gcc -o recvudp recvudp.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o icmp.o dns.o -Wall

icmptest: icmptest.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o icmp.o dns.o 
	gcc -o icmptest icmptest.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o icmp.o dns.o -Wall

testdns: testdns.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o icmp.o dns.o
	gcc -o testdns testdns.o udp.o ip.o ethernet.o mbuf.o tuntap_if.o utils.o icmp.o dns.o -Wall

clean: 
	-rm -f *.o
	-rm -f recvudp testudp icmptest testdns