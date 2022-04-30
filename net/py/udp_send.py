import socket

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

s.sendto("test".encode(), ("10.1.1.6", 12345))