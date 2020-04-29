// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"io"
	"log"
	"net"
	"time"
	"fmt"
	"os"
)

func handler(c net.Conn, zone string) {
	defer c.Close()
	loc, err := time.LoadLocation(zone)

	if err != nil {
		log.Fatal(err)
	}

	for {
		_, err := io.WriteString(c, zone + " > " + time.Now().In(loc).Format("15:04:05\n"))

		if err != nil {
			log.Fatal(err)
			return // e.g., client disconnected
		}

		time.Sleep(1 * time.Second)
	}
}

func main() {
	if len(os.Args) < 3 {
		fmt.Println("Please use the correct format: TZ=<timezone> go run clock2.go -port <port number>")
		os.Exit(1)
	}

	zone := os.Getenv("TZ")
	p := os.Args[2]

	listener, err := net.Listen("tcp", "localhost:"+p)

	if err != nil {
		log.Fatal(err)
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handler(conn, zone)
	}
}
