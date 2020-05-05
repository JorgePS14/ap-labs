package main

import (
	"io"
	"log"
	"net"
	"os"
	"flag"
	"fmt"
)

func main() {
	if len(os.Args) < 5 || len(os.Args) > 5 {
		fmt.Println("Please use the correct format for running the client: ")
		fmt.Println("go run client.go -user <your username> -server localhost:<port>")
		os.Exit(1)
	}

	host := flag.String("server", os.Args[4], "server")
	user := flag.String("user", os.Args[2], "user")

	conn, err := net.Dial("tcp", *host)
	
	if err != nil {
		log.Fatal(err)
	}

	fmt.Fprintf(conn, *user + "\n")

	done := make(chan struct{})

	go func() {
		io.Copy(os.Stdout, conn)
		log.Println("Thanks for using Simple IRC Server. Have a nice day.")
		done <- struct{}{}
	}()

	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done
	os.Exit(1)
}


func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
