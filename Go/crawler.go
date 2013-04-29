package main

import (
	"fmt"
)

type Fetcher interface {
	// Fetch returns the body of URL and
	// a slice of URLs found on that page.
	Fetch(url string) (body string, urls []string, err error)
}

var m map[string]bool
// Crawl uses fetcher to recursively crawl
// pages starting with url, to a maximum of depth.
func Crawl(url string, depth int, fetcher Fetcher, ch chan int) {
	// TODO: Fetch URLs in parallel. --- DONE 
	// TODO: Don't fetch the same URL twice. --- DONE
	// This implementation doesn't do either:
	if depth <= 0 {
		ch <- 0
		return
	}

	if !m[url] {
		m[url] = true
		body, urls, err := fetcher.Fetch(url)

		if err != nil {
			fmt.Println(err)
			ch <- 0
			return
		}
		fmt.Printf("found: %s %q\n", url, body)
		c := make(chan int, len(urls))
		for _, u := range urls {
			go Crawl(u, depth-1, fetcher, c)
		}

		for i := 0; i < len(urls); i++ {
			<-c
		}
	}
	ch <- 0
	return
}

func main() {
	m = make(map[string]bool)
	//res, _ := (*fetcher)["http://golang.org/"]
	//urls := res.urls
	c := make(chan int, 2)
	Crawl("http://golang.org/", 4, fetcher, c)
	<-c
	//<- c
	//return
}

// fakeFetcher is Fetcher that returns canned results.
type fakeFetcher map[string]*fakeResult

type fakeResult struct {
	body string
	urls []string
}

func (f *fakeFetcher) Fetch(url string) (string, []string, error) {
	if res, ok := (*f)[url]; ok {
		return res.body, res.urls, nil
	}
	return "", nil, fmt.Errorf("not found: %s", url)
}

// fetcher is a populated fakeFetcher.
var fetcher = &fakeFetcher{
	"http://golang.org/": &fakeResult{
		"The Go Programming Language",
		[]string{
			"http://golang.org/pkg/",
			"http://golang.org/cmd/",
		},
	},
	"http://golang.org/pkg/": &fakeResult{
		"Packages",
		[]string{
			"http://golang.org/",
			"http://golang.org/cmd/",
			"http://golang.org/pkg/fmt/",
			"http://golang.org/pkg/os/",
		},
	},
	"http://golang.org/pkg/fmt/": &fakeResult{
		"Package fmt",
		[]string{
			"http://golang.org/",
			"http://golang.org/pkg/",
		},
	},
	"http://golang.org/pkg/os/": &fakeResult{
		"Package os",
		[]string{
			"http://golang.org/",
			"http://golang.org/pkg/",
		},
	},
}
