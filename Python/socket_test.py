import socket

def get_constant(prefix):
    return dict( (getattr(socket, n), n) 
                for n in dir(socket) 
                if n.startswith(prefix)
                )

families = get_constant("AF_")
types = get_constant("SOCK_")
protocols = get_constant("IPPROTO_")

for response in socket.getaddrinfo('www.foreric.com','http'):
    family, socktype, proto, cannonname, sockadr = response

    print 'Family:       :', families[family]
    print 'Type:         :', types[socktype]
    print 'Protocol      :', protocols[proto]
    print 'Canonical name:', cannonname
    print 'Socket Address:', sockadr
    print ''
