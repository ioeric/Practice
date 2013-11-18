package skiplist

import (
	"fmt"
	"math"
	"math/rand"
	"time"
)

type Key uint32
type Value string

const MAX_HEIGHT int = 20
const P float32 = 1 / 2.0

type Node struct {
	key     Key
	value   Value
	height  int
	forward [MAX_HEIGHT]*Node
}

type SkipList struct {
	level int
	head  *Node
	tail  *Node
}

func (list *SkipList) findNodeBefore(key Key) (*Node, []*Node) {
	x := list.head
	record := make([]*Node, list.level)
	for lv := list.level - 1; lv >= 0; lv-- {
		fmt.Printf("Key = %v, height = %v\n", x.key, x.height)
		for x.forward[lv].key < key {
			x = x.forward[lv]
		}
		record[lv] = x
	}
	return x, record
}

func (list *SkipList) Insert(key Key, value Value) {
	nodeBefore, record := list.findNodeBefore(key)
	node := nodeBefore.forward[0]

	if node.key == key {
		fmt.Printf("Node with key [%v] already exists, update value to %v", key, value)
		node.value = value
		return
	}

	randomHeight := generateRandomHeight()
	newNode := NewNode(randomHeight, key, value, nil)
	
	minHeight := randomHeight
	
	if randomHeight > list.level {
		fmt.Printf("Update level from %v to %v", list.level, randomHeight)
		minHeight = list.level
		for lv := randomHeight - 1; lv >= list.level; lv-- {
			list.head.forward[lv] = newNode
			newNode.forward[lv] = list.tail
		}
		list.level = randomHeight
	}
	
	for lv := minHeight - 1; lv >= 0; lv-- {
		newNode.forward[lv] = record[lv].forward[lv]
		record[lv].forward[lv] = newNode
	}
}

func (list *SkipList) Search(key Key) (Value, error) {
	nodeBefore, _ := list.findNodeBefore(key)
	node := nodeBefore.forward[0]

	if node.key != key {
		return "", fmt.Errorf("No KVP with key = %v", key)
	}
	return node.value, nil

}

func (list *SkipList) Delete(key Key) error {
	nodeBefore, record := list.findNodeBefore(key)
	node := nodeBefore.forward[0]

	if node.key != key {
		return fmt.Errorf("No KVP with key = %v", key)
	}

	for lv := list.level - 1; lv >= 0; lv-- {
		if record[lv].forward[lv] == node {
			record[lv].forward[lv] = node.forward[lv]
		}
	}

	return nil
}

func (list *SkipList) Print() {
	node := list.head
	for node != nil {
		fmt.Printf("key = %v, value = %v, height = %v \n", node.key, node.value, node.height)
		node = node.forward[0]
	}
}

func generateRandomHeight() int {
	h := 1
	for rand.Float32() < P {
		h++
	}
	if h > MAX_HEIGHT {
		h = MAX_HEIGHT
	}
	return h
}
func NewNode(h int, key Key, value Value, nextNode *Node) *Node {
	if h > MAX_HEIGHT {
		h = MAX_HEIGHT
	}

	nd := new(Node)
	nd.height = h
	nd.key = key
	nd.value = value
	for i := 0; i < h; i++ {
		nd.forward[i] = nextNode
	}
	return nd 
}

func NewSkipList(initHeight int) *SkipList {
	sd := int64(time.Now().Second())
	rand.Seed(sd)
	sl := new(SkipList)
	sl.level = initHeight
	sl.tail = NewNode(initHeight, math.MaxUint32, "HEAD", nil)
	sl.head = NewNode(initHeight, 0, "TAIL", sl.tail)
	return sl
}

func main() {
	sl := NewSkipList(4)
	var n int
	var key Key
	var value Value
	fmt.Scanf("%d", &n)
	for i := 0; i < n; i++ {
		fmt.Scanf("%d %s", &key, &value)
		sl.Insert(key, value)
	}
	sl.Print()
}
