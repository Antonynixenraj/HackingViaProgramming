import socket
from threading import Thread

server_host = '127.0.0.1' # TODO : Replace the ip with actual server ip.
server_port = 1234

class Client:
    def __init__(self,server_host,server_port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((server_host, server_port))

        # Send the client name
        print("\n🌟 Welcome to the Group Chat Room! 🌟\nCreated by Antony Nixen Raj 💻\nThis is a Python socket programming project for real-time group chat across multiple clients.\n")
        self.client_name = input("To begin, please enter your name to join the chat: ")
        self.sock.send(self.client_name.title().encode())
        print()


    def receive_messages(self):
        try:
            while True:
                self.message = self.sock.recv(1024).decode()
                if not self.message:
                    break
                print(self.message)
        except :
            pass

        self.sock.close()

    def send_messages(self):
        try:
            while True:
                self.message = input()
                self.sock.send(self.message.encode())
                if self.message.lower() == "exit":
                    break
        except:
            pass

        self.sock.close()


client = Client(server_host,server_port)

Thread(target=client.receive_messages).start()
Thread(target=client.send_messages).start()


