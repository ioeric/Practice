package extHash

import (
	"fmt"
	"os"
)

type Key uint8
type Value string
const MAX_DIR_DEPTH int = 64
type KVP struct {
	key Key
	value Value
}

type Bucket struct {
	depth int
	size int
	kvps []*KVP	
}

type Directory struct {
	depth int
	size int
	buckets []*Bucket
}

type Table struct {
	dir *Directory
	maxSize int
}

func getFirstNDigits(x Key, n int) int {
	return int(x >> uint(8-n))
}

func (t *Table) Insert(key Key, value Value) error {
	kvp, _ := t.Search(key)
	
	if kvp != nil {
		kvp.value = value	
		return nil
	}

	pos := getFirstNDigits(key, t.dir.depth)
	bucket := t.dir.buckets[pos]
	
	// If the bucket is full and the depth of it is less than the depth of the directory,
	// keep splitting
	for bucket.size >= t.maxSize && bucket.depth < t.dir.depth {
		dif := t.dir.depth - bucket.depth
		// Number of pointers pointing to this bucket
		m := 1 << uint(dif)
		st := pos - (pos % m)
		newDepth := bucket.depth + 1	
		bucket1 := NewBucket(newDepth, t.maxSize)
		bucket2 := NewBucket(newDepth, t.maxSize)
		
		newM := m / 2
		for i := 0; i < newM; i++ {
			t.dir.buckets[st + i] = bucket1
		}

		for i := newM; i < m; i++ {
			t.dir.buckets[st + i] = bucket2
		}

		for i := 0; i < bucket.size; i++ {
			p := getFirstNDigits(bucket.kvps[i].key, t.dir.depth)
			t.dir.buckets[p].kvps[ t.dir.buckets[p].size ] = bucket.kvps[i]
			t.dir.buckets[p].size ++
		}
		
		bucket = t.dir.buckets[pos]
	}
	
	if (bucket.size < t.maxSize) {
		bucket.kvps[bucket.size] = &KVP{key, value}
		bucket.size ++
		return nil
	} else {
		// The bucket is still full after all possible splitting, expand directory
		if t.dir.depth >= MAX_DIR_DEPTH {
			err := fmt.Errorf("Cannot expand directory anymore! depth = %v\n", t.dir.depth)
			return err
		}

		newBuckets := make([]*Bucket, t.dir.size * 2)
		for i := 0; i < t.dir.size; i++ {
			newBuckets[i*2] = t.dir.buckets[i]
			newBuckets[i*2 + 1] = t.dir.buckets[i]
		}
		t.dir.buckets = newBuckets
		t.dir.depth ++
		t.dir.size = t.dir.size * 2
		err := t.Insert(key, value)
		return err
	}

}

func (t *Table) Search(key Key) (*KVP, error) {
	pos := getFirstNDigits(key, t.dir.depth)
	if (pos >= t.dir.size) {
		fmt.Printf("Internal ERROR: pos(%v) >= t.dir.size(%v)",
			pos, t.dir.size)
		os.Exit(1)
	}

	bucket := t.dir.buckets[pos]
	for i := 0; i < bucket.size; i++ {
		if bucket.kvps[i].key == key {
			return bucket.kvps[i], nil
		}
	}
	return nil, fmt.Errorf("Key %v not found!", key)
}

func (t *Table) shrink(pos int) error {
	/*
	bucket := t.dir.buckets[pos]
	dif := t.dir.depth - bucket.depth
	// Number of pointers pointing to this bucket
	m := 1 << dif
	st := pos - (pos % m)
	for i := 0; i < m; i++ {
		t.dir.buckets[st + i] = 
	}

	*/
	return nil
}

func (t *Table) Delete(key Key) error {
	pos := getFirstNDigits(key, t.dir.depth)
	if (pos >= t.dir.size) {
		fmt.Printf("Internal ERROR: pos(%v) >= t.dir.size(%v)",
			pos, t.dir.size)
		os.Exit(1)
	}

	bucket := t.dir.buckets[pos]
	for i := 0; i < bucket.size; i++ {
		if bucket.kvps[i].key == key {
			bucket.kvps[i] = bucket.kvps[bucket.size - 1]
			bucket.size --
			
			if bucket.size == 0 {
				t.shrink(pos)
			}

			return nil
		}
	}

	return fmt.Errorf("Key %v not found!", key)
}

func NewBucket(d int, s int) *Bucket {
	bkt := &Bucket{}
	bkt.depth = d
	bkt.kvps = make([]*KVP, s)
	bkt.size = 0
	return bkt
}

func NewTable(s int) *Table {
	tb := new(Table)
	tb.maxSize = s
	tb.dir = &Directory{depth:0, size:1, buckets:make([]*Bucket, 1)}
	tb.dir.buckets[0] = NewBucket(0, 4)
	tb.dir.buckets[0].depth = 0
	tb.dir.buckets[0].size = 0
	tb.dir.buckets[0].kvps = make([]*KVP, s)
	return tb
}
/*
func main() {
	n := 100
	ext := NewTable(4)
	m := make(map[Key]Value)
	flags := make(map[Key]bool)
	rand.Seed(int64(time.Now().Second()))
	
	k := Key(rand.Uint32() % 256)
	v := Value(strconv.Itoa( int(k) ))
	
	for n > 0 {
		for flags[k] {
			k = Key(rand.Uint32() % 256)
			v = Value(strconv.Itoa( int(k) ))
		}	
		m[k] = v
		fmt.Printf("Inserting key = %v \n",k)
		err := ext.Insert(Key(k),v)
		if err != nil {
			//t.Fatal(err)
			fmt.Println(err)
			os.Exit(1)
		}
		fmt.Printf("Inserted key = %v\n",k)
		k = Key(rand.Uint32() % 256)
		v = Value(strconv.Itoa( int(k) ))
		n--
	}
		
	for k, _ := range m {
		q, err := ext.Search(k)
		if err != nil {
			//t.Fatal(err)
			fmt.Println(err)
			os.Exit(1)
		}
		
		if q.value != m[k] {
			fmt.Printf("Key %v has value = %v in table, value = %v in map\n", q, m[k])
			os.Exit(1)
		}
		//fmt.Printf("In map: key = %v value = %v; In ext table: key = %v value = %v\n", 
		//	k, m[k], q.key, q.value)
	}


	for k, _ := range m {
		if k % 2 == 0 {
			ext.Delete(k)
			delete(m, k)
		}
	}
	fmt.Println("After delete")	
	for k, _ := range m {
		q, err := ext.Search(k)
		if err != nil {
			//t.Fatal(err)
			fmt.Printf("ERROR: %v",err)
			os.Exit(1)
		}
		
		if q.value != m[k] {
			fmt.Printf("Key %v has value = %v in table, value = %v in map\n", q, m[k])
			os.Exit(1)
		}
		fmt.Printf("In map: key = %v value = %v; In ext table: key = %v value = %v\n", 
			k, m[k], q.key, q.value)
	}

}

*/
