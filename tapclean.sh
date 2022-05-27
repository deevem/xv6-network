brctl delif br0 tap0

tunctl -d tap0

brctl delif br0 enx00e04c360838

ifconfig br0 down

brctl delbr br0

ifconfig enx00e04c360838 up

dhclient -v enx00e04c360838