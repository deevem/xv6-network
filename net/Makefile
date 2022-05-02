%.o : %.c
	gcc -c $^ -Wall

NETOBJECT = \
	$Kethernet.o \
	$Kip.o \
	$Karp.o \
	$Kudp.o \
	$Kmbuf.o \
	$Ktuntap_if.o \
	$Kutils.o \
	$Kicmp.o \
	$Kdns.o \

testudp: testudp.o $(NETOBJECT)
	gcc -o testudp testudp.o $(NETOBJECT) -Wall

recvudp: recvudp.o $(NETOBJECT)
	gcc -o recvudp recvudp.o $(NETOBJECT) -Wall

icmptest: icmptest.o $(NETOBJECT)
	gcc -o icmptest icmptest.o $(NETOBJECT) -Wall

testdns: testdns.o $(NETOBJECT)
	gcc -o testdns testdns.o $(NETOBJECT) -Wall

arptest: arptest.o $(NETOBJECT)
	gcc -o arptest arptest.o $(NETOBJECT) -Wall

clean: 
	-rm -f *.o
	-rm -f recvudp testudp icmptest testdns