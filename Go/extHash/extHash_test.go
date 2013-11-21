package extHash

import (
	"testing"
	"math/rand"
	"time"
	"strconv"
)

func TestIt(t *testing.T) {
	n := 1000
	ext := NewTable(4)
	m := make(map[Key]Value)
	flags := make(map[Key]bool)
	rand.Seed(int64(time.Now().Second()))
	
	k := Key(rand.Uint32() % 256)
	v := Value(strconv.Itoa( int(k) ))
	
	for n >0 {
		for flags[k] {
			k = Key(rand.Uint32() % 256)
			v = Value(strconv.Itoa( int(k) ))
		}	
		m[k] = v
		err := ext.Insert(Key(k),v)
		if err != nil {
			t.Fatal(err)
		}
		n--
		k = Key(rand.Uint32() % 256)
		v = Value(strconv.Itoa( int(k) ))
	}
	
	for k, _ := range m {
		q, err := ext.Search(k)
		if err != nil {
			t.Fatal(err)
		}

		if q.value != m[k] {
			t.Fatal("Key %v has value = %v in table, value = %v in map", q, m[k])
		}
	}
	t.Logf("# of KVPs = %v", len(m))
}
