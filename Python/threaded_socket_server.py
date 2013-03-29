import threading
import SocketServer

class ThreadedRequestHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        data = self.request.recv(1024)
        cur_thread = threading.currentThread()
        print 'Server received "%s":"%s"' %(cur_thread.getName(),data)
        reponses = '%s: %s' %(cur_thread.getName(), data)
        self.request.send(responses)
        return
class ThreadedSocketServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

if __name__ == '__main__':
    import socket
    import threading
    
    address = ('localhost', 0)
    server = ThreadedSocketServer(address, ThreadedRequestHandler)
    ip, port = server.server_address
    print 'Server listens %s:%s' %(ip, port) 
    t = threading.Thread(target=server.serve_forever)
    t.setDaemon(True)
    t.start()
    print 'Server runs on %s' %t.getName()
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((ip, port))

    message = 'Hello, Waterloo!'
    print 'Sending message "%s"' %message
    len_sent = s.send(message)
    print 'Sent "%s", length:%d' %(message, len_sent)
    response = s.recv(1024)
    print 'Received: "%s"' %response 
    print 'Closing socket' 
    s.close()
    print 'Closing server socket'
    server.socket.close()
