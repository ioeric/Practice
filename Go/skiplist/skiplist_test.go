package skiplist

import (
	"testing"
	"sort"
	"math/rand"
	"time"
	"fmt"
	"strconv"
)

type KVP struct {
	key Key
	value Value
}

type By func(p1, p2 *KVP) bool

func (by By) Sort(list []KVP) {
	st := &KVPSorter{
		list: list,
		by : by,
	}

	sort.Sort(st)
}

type KVPSorter struct {
	list []KVP
	by By
}

func (s *KVPSorter) Len() int {
	return len(s.list)
}

func (s *KVPSorter) Swap(i,j int) {
	s.list[i], s.list[j] = s.list[j], s.list[i]
}

func (s *KVPSorter) Less(i,j int) bool {
	return s.by(&s.list[i], &s.list[j])
}

func generateList(n int) []KVP {
	flag := make(map[int32] bool)
	list := make([]KVP, n)
	rand.Seed(int64(time.Now().Second()))
	i := 0
	for i < n {
		r := rand.Int31() + 1
		_, ok := flag[r]
		for ok {
			r = rand.Int31() + 1	
			_, ok = flag[r]
		}
		flag[r] = true
		list[i].key = Key(r)
		list[i].value = Value(strconv.Itoa(int(r)))
		i++
	}
	
	return list
}
func TestInsertAndDelete(t *testing.T)  {
	byKey := func(p1, p2 *KVP) bool {
		return p1.key < p2.key
	}	
	num := 10000
	list := generateList(num)
	sl := NewSkipList(5)
	for _, v := range list {
		fmt.Printf("key = %v, value = %v\n", v.key, v.value)
	}
	for i := range list {
		sl.Insert(list[i].key, list[i].value)
	}
	
	By(byKey).Sort(list)
	k1 := list[ len(list)/2 ].key
	k2 := list[ len(list) - 1 ].key
	sl.Delete(k1)
	sl.Delete(k2)
	sl.Insert(k1, "k1")
	sl.Insert(k2, "k2")
	p := sl.head.forward[0]
	for i := range list {
		fmt.Printf("%v -  %v", list[i], p.key)
		if (list[i].key != p.key) {
			fmt.Printf("%v != %v at pos %v\n", list[i].key, p.key, i)
		}
	}
}
