import select
import socket
import struct
import sys
import Queue

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setblocking(0)
server_address = ('localhost',1992)

print 'starting up on %s port %s' % server_address

server.bind(server_address)
server.listen(5)

inputs = [ server ]
outputs = []

message_queues = {}

while inputs:
    print 'waiting for the next event'
    readable, writable, exceptional = select.select(inputs, outputs, inputs)
    for s in readable:
        if s is server:
            connection, client_address = s.accept()
            print 'new connection from ', client_address
            connection.setblocking(0)
            inputs.append(connection)
            message_queues[connection] = Queue.Queue()
        else:
            data = s.recv(1024)
            if data:
                print 'received "%s" from %s' % (data, s.getpeername())
                message_queues[s].put(data)
                if s not in outputs:
                    outputs.append(s)
            else:
                print 'closing connection from ', s.getpeername()
                if s in outputs:
                    outputs.remove(s)
                inputs.remove(s)
                s.close()
                del message_queues[s]
    for s in writable:
        try:
            next_message = message_queues[s].get_nowait()
        except Queue.Empty:
            print 'output queue for is empty ', s.getpeername()
            outputs.remove(s)
        else:
            print 'sending "%s" to "%s"' %(next_message, s.getpeername())
            s.send(next_message)    
    for s in exceptional:
        print 'handling exceptional connection "%s"' %s.getpeername()
        inputs.remove(s)
        if s in outputs:
            outputs.remove(s)
        s.close()
        del message_queues[s]


