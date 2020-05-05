package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"strings"
	"time"
	"os"
	"flag"
	"bytes"
)

type client chan<-string

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string)
	serverMsg = "Server says> "
	admin string
	onlineUsers map[string]net.Conn
	zone = "America/Mexico_City"
	timezone, _ = time.LoadLocation("America/Mexico_City")
)

func usersCommand() string {
	out := ""

	out += serverMsg + "Users currently connected: \n"
	out += "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"

	for user, _ := range onlineUsers {
		out += user
		if user == admin {
			out += " (Server admin)"
		}
		out += "\n"
	}

	out += "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+"

	return out
}

func msgCommand(origin, destination string, message []string) {
	if _, ok := onlineUsers[destination]; ok {
		fmt.Fprintln(onlineUsers[origin], origin + "(privately to " + destination + ")> " + strings.Join(message, " "))
		fmt.Fprintln(onlineUsers[destination], origin + "(privately to you)> " + strings.Join(message, " "))
		return
	}

	fmt.Fprintln(onlineUsers[origin], serverMsg + strings.Join(message, " "))
}

func timeCommand() string {
	time := time.Now().In(timezone).Format("15:04\n")
	return serverMsg + "Local time: " + zone + " - " + strings.Trim(time, "\n")
}

func userCommand(username string) string {
	if _, ok := onlineUsers[username]; ok {
		return serverMsg + username + ", IP: " + onlineUsers[username].RemoteAddr().String()
	}

	return serverMsg + "The user <" + username + "> doesn't exist"
}

func kickCommand(user, ban string) {
	if strings.Compare(user, admin) != 0 {
		fmt.Fprintln(onlineUsers[user], serverMsg + "Only the session administrator can kick users from the server.")
	} else {
		if _, ok := onlineUsers[ban]; ok {
			fmt.Fprintln(onlineUsers[ban], serverMsg + "The server admin has kicked you for inappropriate behavior.")
			fmt.Println(serverMsg + "The user [" + ban + "] has been kicked")
			messages <- serverMsg + "The user [" + ban + "] has been kicked from the server. Please behave accordingly."
			onlineUsers[ban].Close()
			delete(onlineUsers, ban)
		}

		fmt.Fprintln(onlineUsers[user], serverMsg + "The username " + ban + " doesn't exist in this session.")
	}
}

func broadcaster() {
	clients := make(map[client]bool)
	onlineUsers = make(map[string]net.Conn)

	for {
		select {
			case msg := <-messages:
				for client := range clients {
					client <- msg
				}

			case client := <-entering:
				clients[client] = true

			case client := <-leaving:
				delete(clients, client)
				close(client)
		}
	}
}

func handleConn(conn net.Conn) {

	buffer := make([]byte, 128)
	conn.Read(buffer)
	usr := string(bytes.Trim(buffer, "\x00"))
	usr = strings.Replace(usr, "\n", "", 1)

	ch := make(chan string)

	newAdmin := false
	
	if len(onlineUsers) == 0 {
		newAdmin = true
	}

	if onlineUsers[usr] != nil {
		fmt.Fprintln(conn, "Please choose a different username, " + usr + " is already taken.")
		close(ch)
		conn.Close()
		return
	}

	onlineUsers[usr] = conn

	fmt.Printf("%sA new user has logged into the session: [%s]\n", serverMsg, usr)

	fmt.Fprintln(conn, serverMsg + "Welcome to the Simple IRC Server.")
	fmt.Fprintln(conn, serverMsg + "Login succeeded: " + time.Now().UTC().String())
	fmt.Fprintln(conn, serverMsg + "We hope you have a great time chatting.")
	fmt.Fprintln(conn, serverMsg + "Any behavior deemed inappropriate by the session admin can get you kicked out.")

	if newAdmin {
		admin = usr
		fmt.Fprintln(conn, serverMsg + "You are now the admin of this IRC server.")
	}

	go clientWriter(conn, ch)

	messages <- serverMsg + "A new user has logged into the session: Give [" + usr + "] a warm welcome!"
	entering <- ch

	input := bufio.NewScanner(conn)

	for input.Scan() {
		if len(input.Text()) > 0 {
			if string(input.Text()[0]) == "/" {
				command := strings.Split(input.Text(), " ")
				cmd := command[0]

				switch cmd {
					case "/users":
						ch <- usersCommand()

					case "/msg":
						if len(command) < 3 {
							ch <- serverMsg + "Please use the correct command format: /msg <user> <message>"
							continue
						}

						msgCommand(usr, command[1], command[2:])

					case "/time":
						ch <- timeCommand()

					case "/user":
						if len(command) < 2 {
							ch <- serverMsg + "Please use the correct command format: /user <user>"
							continue
						}

						ch <- userCommand(command[1])

					case "/kick":
						if len(command) < 2 {
							ch <- serverMsg + "Please use the correct command format: /kick <user>"
							continue
						}

						kickCommand(usr, command[1])

					default:
						ch <- serverMsg + "Please refrain from starting your messages with '/'. That character is used for commands only."
				}
			} else {
				messages <- usr + "> " + input.Text()
			}
		}
	}

	leaving <- ch
	fmt.Println(serverMsg + "[" + usr + "] has left.")
	messages <- serverMsg + "[" + usr + "] has left the chat. Goodbye, [" + usr + "]"
	delete(onlineUsers, usr)
	conn.Close()

	if usr == admin {
		for user, _ := range onlineUsers {
			admin = user
			fmt.Println(serverMsg + "[" + user + "] is now the new admin.")
			messages <- serverMsg + "[" + user + "] has been promoted to admin because the previous admin, [" + usr + "], is leaving."
			break
		}
	}
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg)
	}
}

func main() {
	if len(os.Args) < 5 {
		fmt.Println("Please use the correct format: go run server.go -host localhost -port <port>")
		os.Exit(1)
	}

	fmt.Println(serverMsg + "Initializing server...")

	host := flag.String("host", "localhost", "address")
	port := flag.String("port", os.Args[4], "listener")
	flag.Parse()

	listener, err := net.Listen("tcp", *host + ":" + *port)

	if err != nil {
		log.Fatal(err)
	}

	go broadcaster()

	fmt.Println(serverMsg + "Successfully mounted IRC Server in " + *host + ":" + *port)
	fmt.Println(serverMsg + "Server initialized. Ready to receive new clients.")

	for {
		conn, err := listener.Accept()

		if err != nil {
			log.Print(err)
			continue
		}

		go handleConn(conn)
	}

}