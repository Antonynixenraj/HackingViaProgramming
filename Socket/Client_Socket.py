import socket
s=socket.socket()
s.connect(("127.0.0.1",1234))
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