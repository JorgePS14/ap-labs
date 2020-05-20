package main

import (
	"fmt"
	"log"
	"os"
	"flag"

	"gopl.io/ch5/links"
)

var tokens = make(chan struct{}, 20)

func crawl(url string) []string {

	fmt.Println(url)
	tokens <- struct{}{}
	list, err := links.Extract(url)
	<-tokens

	if err != nil {
		log.Print(err)
	}

	return list
}

func main() {

	if len(os.Args) < 3 {
		fmt.Println("Please use the correct format: go run crawl3.go -depth=<int:depth> <url-to-crawl>")
		os.Exit(1)
	}
	worklist := make(chan []string)
	var n int

	n++

	var d int
	flag.IntVar(&d, "depth", 10, "Crawling depth")

	go func() { worklist <- os.Args[1:] }()

	seen := make(map[string]bool)

	for ; n > 0; n-- {
		list := <-worklist
		
		if d > 0 {
			for _, link := range list {
				if !seen[link] {
					seen[link] = true
					n++
					d--
					go func(link string) {
						worklist <- crawl(link)
					}(link)
				}
			}
		}
	}
}

//!-
