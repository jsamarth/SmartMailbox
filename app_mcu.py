import threading
import sys
import socket
import time
import socketserver
from threading import Lock

sock_list = []
bank = {}

def server_socket(port):
	class LoggerHandler(socketserver.BaseRequestHandler):
		def handle(self):
			file = self.request.makefile('r')
			for line in file:
				line = line.replace('\n', '')
				
			print(time.time(), "-", f"{name} disconnected")

	with socketserver.ThreadingTCPServer(('127.0.0.1', port), LoggerHandler) as server:
		server.serve_forever()

if __name__ == "__main__":

	server_thread = threading.Thread(target=server_socket, args=(9876, ), daemon=True)
	server_thread.start()

	sock = socket.create_connection(('192.168.1.147',23))
	sock.send(f"first\n".encode())

	print("sockets created")

	while True:
		time.sleep(2)
		sock.send(f"hey\n".encode())
