package main

import(
	"fmt"
	"io"
	"net"
	"log"
	"strings"
	"os"
)

func handler(host string, wait chan int) {
	conn, err := net.Dial("tcp", host)

	if err != nil {
		log.Fatal(err)
	}

	success := make(chan int)

	go func() {
		io.Copy(os.Stdout, conn)
		log.Println("Success")
			success <- 2
			wait <- 1
	}()

	x := 1
	x = <- success
	log.Println("Channel was closed with value: ", x)
	close(success)
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Please use the correct format: go run clockWall.go <location>=<host>:<port number> ...")
		os.Exit(1)
	}

	hosts := os.Args[1:]
	wait := make(chan int)

	for _, host := range hosts {
		list := strings.Split(host, "=")
		go handler(list[1], wait)
	}

	<-wait
	close(wait)
}