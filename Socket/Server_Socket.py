import socket

s=socket.socket()

s.bind(('',1234))
s.listen(5)
print('Socket is listening.....')
c,add=s.accept()
print('Got connection from ',add)
while(True):
    n=input('>> ')
    c.send(n.encode())
    msg=c.recv(1024).decode()
    if(msg=="exit" or n=="exit"):
        break
    print('$ '+msg)

c.close()