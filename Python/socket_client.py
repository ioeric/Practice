import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('linux032.student.cs', 1992)
print 'Connecting to %s port %s ' % server_address
sock.connect(server_address)
count = 0
while count<100:
    message = raw_input("Enter a message to be sent: ")
    print 'Sending %s' % message
    sock.sendall(message)
    amount_received = 0
    amount_expected = len(message)
    while amount_received < amount_expected:
        data = sock.recv(16)
        amount_received += len(data)
        print 'Received %s' % data

sock.close()
