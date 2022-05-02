import socket

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('', 12345))
while True:
    data = s.recv(1024)
    print(data.decode())