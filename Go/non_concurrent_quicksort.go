package main

import (
	"fmt"
	"time"
	"math/rand"
	"sort"
)

const numElements = 100000000
const maxElement = 1000000000
func RandomArray (n, max int) []int {
	arr := make([]int, n)
	for i := 0; i < n; i++ {
		arr[i] = rand.Intn(max)
	}
	return arr
}

func main() {
	arr := RandomArray(numElements, maxElement)
	t := time.Now()
	sort.Ints(arr)
	dur := time.Since(t)
	fmt.Printf("Non-current sort took %.3f to sort %d integers\n", dur.Seconds(), numElements)
	sorted := true
	for i:=0;i<len(arr)-1;i++{
		if arr[i]>arr[i+1]{
			sorted = false
			break
		}
	}

	if sorted {
		fmt.Println("Array sorted!")
	} else {
		fmt.Println("ERROR: array has not been sorted correctly!")
	}
	
}
