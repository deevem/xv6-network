import socket

def GetHostIP():
    try:
        Socke = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        Socke.connect(('8.8.8.8', 80))
        IP = Socke.getsockname()[0]
    finally:
        Socke.close()
 
    print(IP)
    return IP

x = GetHostIP()
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

s.sendto("test".encode(), ("127.0.1.1", 12345))