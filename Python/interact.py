from __future__ import print_function

f = open('test_large.in')
t = int(f.readline())

for i in range(t):
    n = int(f.readline())
    dic = {}
    for j in range(n):
        x, y = [int(t) for t in f.readline().split()]
        dic[x] = y
    
    kiss = sorted(dic)
#    print('kiss ',kiss)
    res = 0
    for t in range(len(kiss)-1, 0, -1):
        #       print(t,' : ',kiss[t],':',dic[kiss[t]])
        for j in range(t-1, -1, -1):
            #   print(j,' : ',kiss[j],':',dic[kiss[j]])
            if dic[ kiss[t] ] < dic[ kiss[j] ]:
                res = res + 1
             #   print('Inc res', res)
    print('Case #', i+1,': ', res, sep='')
    
f.close()




