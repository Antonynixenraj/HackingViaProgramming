import socket
s=socket.socket()
s.connect(("172.18.8.245",1234))
print("Connected.....")

while(True):
    msg=s.recv(1024).decode()
    if(msg=="exit"):
        break
    print('$ '+msg)
    txt=input('>> ')
    msg_send=s.send(txt.encode())
    if(txt=="exit"):
        break
s.close()