import socket
from threading import Thread
from datetime import datetime

host = ''
port = 1234

class ChatServer:
    clients = {} # To store client address and their socket
    def __init__(self):
        """To make Connection"""
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.bind((host, port))
        self.s.listen(100)       # TODO : It listens upto 100 Clients to connect
        print("Server is listening...")
        self.run_server()

    def run_server(self):
        """To run a server"""
        try:
            while True:
                client_socket, client_address = self.s.accept()
                # Start a thread to handle this client
                Thread(target=self.handle_client, args=(client_socket, client_address)).start()
        except:
            pass

        self.s.close()

    def handle_client(self, client_socket, client_address):
        """Handle communication with a single client"""
        try:
            # Receive and set the client name
            client_name = client_socket.recv(1024).decode()
            self.clients[client_address] = (client_socket, client_name)
            print(f'{datetime.now().strftime("[%Y-%m-%d %H:%M:%S]")} 🔗 {client_name} has joined the chat.')

             # Start receiving messages
            while True:
                message = client_socket.recv(1024)
                if not message or message.decode() == "exit":
                    break
                self.broadcast_message(client_address, message)
        except:
            pass

        if client_address in self.clients:
            print(f'{datetime.now().strftime("[%Y-%m-%d %H:%M:%S]")} ❌ {self.clients[client_address][1]} has left the chat.')
            del self.clients[client_address]
        client_socket.close()

    def broadcast_message(self, sender_address, message):
        """Send a message to all clients except the sender."""
        sender_name = self.clients[sender_address][1]
        full_message = f"🧑‍💻 {sender_name} : {message.decode()}"

        for addr in self.clients.keys():
            if addr != sender_address:  # Do not send to the sender
                try:
                    self.clients[addr][0].send(full_message.encode()) # self.clients[addr][0] -> Returns Socket of address from dictionary
                except:
                    self.clients[addr][0].close()
                    del self.clients[addr]


ChatServer()