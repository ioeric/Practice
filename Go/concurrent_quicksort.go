/*
   Concurrent quicksort 
   Balancer/Worker model
   By Eric Liu
   Let me know if you find any bugs, thanks!
   Please do not delete this comment!
*/

package main

import (
	"fmt"
	"math/rand"
	"time"
)

const numWorkers = 10
const numRequests = 50000
const Limit = 100
const numElements = 10000000
const maxElement = 100000000

// If the number of elements to be sorted is smaller than 100, implement 
//bubble sort
func bubble_sort(arr []int, st, ed int) int {
	if st >= ed {
		return ed - st +1
	}

	flag := false
	for i:=ed;i>=st+1;i-- {
		for j:=st;j<i;j++ {
			if arr[j] > arr[j+1]{
				flag = true
				arr[j], arr[j+1] = arr[j+1], arr[j]
			}
		}
		if !flag {
			break
		}
	} 
	return ed - st + 1
}

// If the number of elements to be sorted is greater than 100, implementing 
// quicksort
func quicksort(arr []int, st, ed int) int {
	if st >= ed {
		return -1
	}
	
	pivot := st
	
	for i := st+1; i <= ed; i++ {
		if arr[i] < arr[st] {
			pivot++
			if pivot != i {
				arr[i], arr[pivot] = arr[pivot], arr[i]	
			}
		}
	}

	if pivot != st {
		arr[st], arr[pivot] = arr[pivot], arr[st]
	} 

	return pivot
}

// Balancer dispatches request(job) to workers
type Request struct {
	arr []int 	// Array to be sorted
	st, ed int      // Start point and end point
}

// Worker receives requests from balancer via 'requests' channel and 
// process the request
type Worker struct{
	requests chan Request 
	idx int   // Worker's index in balancer's pool
	newTask chan Request
	num int   // Record how many requests this worker have processed
}

// Workers keep looping to get a request and process the request after 
// getting one
func (w *Worker) work(done chan int, kill chan int){
	for {
		select{
		case req := <-w.requests: 
			w.num ++ 
			if req.st > req.ed {
				done <- 0
				continue
			} else if req.st == req.ed {
				done <- 1
				continue
			} 

			if req.ed - req.st + 1 >= 100 {
				pivot := quicksort(req.arr, req.st, req.ed)
				newRequest_left := Request{req.arr, req.st, pivot - 1}	
				newRequest_right := Request{req.arr, pivot + 1, req.ed}
				done <- 1
				w.newTask <- newRequest_left
				w.newTask <- newRequest_right
			} else {
				sorted := bubble_sort(req.arr, req.st, req.ed)
				if sorted >= 0 {
					done <- sorted
				} else {
					done <- 0
				}
			}
		case <-kill:	// Suicide!
			return
		}
	}
}

// Pool is a pool of Workers
type Pool []*Worker


func (p *Pool)Append (w *Worker) {
	a := *p
	n := len(a)
	a = a[0 : n+1]
	a[n] = w
	w.idx = n
	*p = a
}


// Balancer maintain 
type Balancer struct {
	p Pool
	done chan int
	pos int
	n int
}

// Initialize a balancer and create its workers
func NewBalancer(work chan Request, n int, done, kill chan int) *Balancer {
	b := &Balancer{ make([]*Worker, 0, numWorkers), done, 0, n}
	pool := &b.p
	for i:=0;i<numWorkers;i++ {
		w := &Worker{make(chan Request, numRequests), i, work, 0}
		pool.Append(w)
		go w.work(done, kill)
	}

	return b
}

// Balancer keep looping to dispatch new requests to workers and get 
// feedback from workers
func (b *Balancer) balance(work chan Request, kill chan int) {
	//fmt.Println("Starting balance")
	sorted := 0
	for {	
		select {
		case req := <-work:
			b.dispatch(req)
		case d := <-b.done:
			sorted = sorted + d
			// If all elements are sorted, finish balance
			if sorted >= b.n {	
				return
			}
		}
	}
}

// Balancer dipatches requests( Round-robbin scheduling for simplicity)
func (b *Balancer) dispatch(req Request) {
	w := b.p[b.pos]
	b.pos++
	w.requests <- req
	if b.pos >= len(b.p) {
		b.pos = 0
	}
	return 
}

func RandomArray (n, max int) []int {
	arr := make([]int, n)
	for i := 0; i < n; i++ {
		arr[i] = rand.Intn(max)
	}
	return arr

}

func isSorted(arr []int) bool {
	for i := 0; i< len(arr)-1; i++ {
		if arr[i] > arr[i+1] {
			return false
		}
	}	
	return true
}

func FirstRequest(arr []int, c chan Request) {
	req := Request{arr, 0, len(arr)-1}		
	c <- req
}

func main() {
	arr := RandomArray(numElements, maxElement)
	work := make(chan Request) // Request channel: send request to balancer	
	done := make(chan int, numWorkers) // Passing the number of elements to balancer sorted after every sort
	kill := make(chan int, numWorkers) // balancer sends terminal signal using kill channel
	b := NewBalancer(work, len(arr), done, kill)
	
	go FirstRequest(arr, work)

	t := time.Now()
	b.balance(work,kill)
	dur := time.Since(t)	
	
	// Send signals to kill all workers
	for i := range b.p {
		kill <- i
	}

	for i,w := range b.p {
		fmt.Printf("Worker %d: Handled %d requests\n", i, w.num)
	}

	fmt.Printf("[Concurrent quicksort]Time used: %.3f\n", dur.Seconds())	
	if isSorted(arr) {
		fmt.Println("Array sorted!")
	} else {
		fmt.Println("ERROR: Array not sorted!")
	}
}
