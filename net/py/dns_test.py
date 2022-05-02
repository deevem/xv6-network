import socket
import struct


s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('', 12345))
while True:
    data = s.recv(1024)

    print("dns recved")

    s.sendto(data, ('114.114.114.114', 53))

    resp_data,(resp_addr,resp_port) = s.recvfrom(512)

    s.sendto(resp_data, ("10.1.1.6", 53))

