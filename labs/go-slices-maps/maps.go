package main

import(
	"strings"
	"golang.org/x/tour/wc")

func CountWords(str string) map[string] int {
	var count map[string] int
	var wordArr []string

	count = make(map[string] int)
	wordArr = strings.Fields(str)

	for _, word := range wordArr {
		count[word] += 1
	}

	return count
}

func main() {
	wc.Test(CountWords)
}
