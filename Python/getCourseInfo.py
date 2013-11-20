# For checking course infomation on Quest (at the University of Waterloo)
# By ioeric (Eric Liu)
# Might need some minor changes for some courses.
import mechanize
import sys
from HTMLParser import HTMLParser
import gtk
import time

res = []

class MyHTMLParser(HTMLParser):
    def handle_starttag(self, tag, attrs):
        pass
        #print "Start tag:",  tag
        #for attr in attrs:
        #    print "\tattrs:", attrs
    
    def handle_endtag(self, tag):
        #print "End tag :", tag
        pass
    
    def handle_data(self, data):
        #print 'data :[%s]' %data
        try:
            x = int(data)
            res.append(x)    
        except Exception:
            pass

if len(sys.argv) < 3:
    print "Wrong number of arguments!"
    sys.exit(1)
args = sys.argv
br = mechanize.Browser()
br.addheaders = [('User-agent','Firefox')]
#print res
while True:
    res = []
    resp1 = br.open('http://www.adm.uwaterloo.ca/infocour/CIR/SA/under.html')
    br.select_form(nr=0)
    br.form['subject'] = [ args[1] ] 
    br.form['cournum'] = args[2]
    resp = br.submit()
    html = resp.read()
    parser = MyHTMLParser()
    parser.feed(html)
    res = res[1:]
    acpt = [1,2,3,4]
    print res
    for i,value in enumerate(res):
        if value in acpt:
            if res[i+1] > res[i+2]:
                print "ENROL!!!"
                d = gtk.Dialog()
                d.add_buttons(gtk.STOCK_YES, 1)
                label = gtk.Label('%s is enrollable!!!')
                label.show()
                d.vbox.pack_start(label)
                d.run()
            else:
                print res[i+1], res[i+2]
    time.sleep(30)
