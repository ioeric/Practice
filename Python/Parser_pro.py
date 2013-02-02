import urllib2
from sgmllib import SGMLParser
import pdb
import re
import sys
import copy
Special_List = ('.php','.aspx','.htm','.html','.asp')
class MyParser(SGMLParser):
	def reset(self):
		SGMLParser.reset(self)
		self.urls = []
        	self.title =self.data= None
	def start_a(self,attrs):
		href = [v for k,v in attrs if k == "href" and re.search("^http.*nju.edu.cn",v)]
		if href: self.urls.extend(href)

        def formalize(self):
		if self.urls:
			for i in range(0,len(self.urls)):
				while self.urls[i][len(self.urls[i])-1] == ' ':
					self.urls[i] = self.urls[i][0:len(self.urls[i])-1]
				Mark = True
				for piece in Special_List:
					if piece in self.urls[i]:
						Mark = False
						break
			  	if '.php/' in self.urls[i]:
					Mark = True
				if Mark and self.urls[i][len(self.urls[i])-1]!='/':
					self.urls[i] += '/'

urls_main_list = []
sock = urllib2.urlopen("http://www.nju.edu.cn/html/yxbm/yx/")
parser = MyParser()
parser.feed(sock.read())
sock.close()
parser.close()
#for i in parser.urls: print i
#print len(parser.urls)
parser.formalize()
pattern = "^http://.*nju.edu.cn/"

for i in parser.urls:
	if re.search(pattern,i) and i not in urls_main_list:
		urls_main_list.append(i)


urls_map = {"http://www.nju.edu.cn/html/yxbm/yx/":urls_main_list}

ErrorList = ['http://museum.nju.edu.cn/', 'http://chin.nju.edu.cn/', 'http://water.nju.edu.cn/', 'http://geoinformatics.nju.edu.cn/', 'http://semlab.nju.edu.cn/', 'http://arch.nju.edu.cn/', 'http://acec.nju.edu.cn/', 'http://grawww.nju.edu.cn/Fxljy/fwxz.htm', 'http://jw.nju.edu.cn/JiaoGai/YanJiuDongTai.asp', 'http://www.nju.edu.cn/cps/site/GH/gh/', 'http://go.nju.edu.cn/xsh/', 'http://go.nju.edu.cn/njuyh/', 'http://go.nju.edu.cn/qing/', 'http://tuanwei.nju.edu.cn/shetuan/', 'http://sgos.nju.edu.cn/hai.html', 'http://hwxy.nju.edu.cn/China/Default.aspx', 'http://ese.nju.edu.cn/', '512 http://nubs.nju.edu.cn/jxjw.php/j11/', 'http://cslab.nju.edu.cn/', 'http://public.nju.edu.cn/', 'http://public.nju.edu.cn/dept/zzxx.aspx', 'http://public.nju.edu.cn/dept/xzgl.aspx', 'http://public.nju.edu.cn/dept/ldsh.aspx', 'http://public.nju.edu.cn/dept/sxzz.aspx']
count = 0
#pdb.set_trace()
new_urls = []

for url in urls_main_list:
	#pdb.set_trace()
	print 'This is ',count,' ',url
	if url in ErrorList:
		count+=1
		continue
	pars = MyParser()
	try:
		sock = urllib2.urlopen(url)
	except urllib2.URLError,e:
		#pdb.set_trace()
		print "Error is in ",url,e
		count+=1
		ErrorList.append(url)
		continue
	pars.feed(sock.read())
	sock.close()
	pars.close()
	pars.formalize()
	my_list = [i for i in pars.urls]
	new_urls+=my_list
	count+=1
urls_main_list = list(set(urls_main_list+new_urls))
print len(urls_main_list)
print urls_main_list
count = 0
M = []
for i in range(0,len(urls_main_list)):
	M.append([])	
	for j in range(0,len(urls_main_list)):
		M[i].append(0)
		

for url in urls_main_list:
	#pdb.set_trace()
	print 'This is ',count,'  of ', len(urls_main_list),url
	if url in ErrorList:
		count+=1
		continue
	pars = MyParser()
	try:
		sock = urllib2.urlopen(url)
	except urllib2.URLError,e:
		#pdb.set_trace()
		count+=1
		print "Error is in ",url,e
		continue
	pars.feed(sock.read())
	sock.close()
	pars.close()
	pars.formalize()

	my_list = [i for i in pars.urls if i in urls_main_list]
	for i in range(0,len(urls_main_list)):
		if urls_main_list[i] in my_list:
			M[count][i] = 1
	print M[count]
	urls_map[url] = my_list
	count+=1
	print "This is ", url, ' which points to ', my_list

ErrorOut = open('Error_pro.out','w')


tmp = sys.stdout
sys.stdout = ErrorOut
print ErrorList
ErrorOut.close()

fout = open('data_pro.out','w')
sys.stdout = fout
print len(M)
for i in M:
	print ''
	for j in i:
		print j,
print '' 
fout.close()
fout_url = open('urls_pro.out','w')

sys.stdout = fout_url
print len(urls_main_list)
for i in range(0,len(urls_main_list)):
	print  urls_main_list[i]

sys.stdout = tmp
fout_url.close()
