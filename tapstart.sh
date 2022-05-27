brctl addbr br0

ip addr flush dev enx00e04c360838

brctl addif br0 enx00e04c360838

tunctl -t tap0

brctl addif br0 tap0

ifconfig enx00e04c360838 up
ifconfig tap0 up
ifconfig br0 up

brctl show

dhclient -v br0