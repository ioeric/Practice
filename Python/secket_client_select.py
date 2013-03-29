import socket
import sys

messages = ['Hello ', 'World! ', 'I am ioeric!']

server_address = ('localhost',1992)
socks = [socket.socket(socket.AF_INET, socket.SOCK_STREAM),
        socket.socket(socket.AF_INET, socket.SOCK_STREAM),
        socket.socket(socket.AF_INET, socket.SOCK_STREAM),
        ]
print 'connecting to server %s on %s ' %server_address

for s in socks:
    s.connect(server_address)

for msg in messages:
    for s in socks:
        print '"%s" sending "%s" to server' % (s.getsockname(), msg)
        s.send(msg)
    for s in socks:
        data = s.recv(1024)
        print '"%s" received "%s" from "%s"' %(s.getsockname(), data, s.getpeername())
        if not data:
            print 'closing socket', s.getsockname()
            s.close()

for s in socks:
    print 'closing socket ', s.getsockname()
    s.close()
