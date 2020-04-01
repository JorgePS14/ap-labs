package main

import (
	"fmt"
	"os"
	"path/filepath"
)

type Dir struct {
	name string
	dirs int
	symLinks int
	devs int
	sockets int
	other int
}

func printPathData(dir Dir) {
	fmt.Println("Directory Scanner Results:")
	fmt.Println("+-------------------------+------+")
	fmt.Printf("| Path                     | %s |\n", dir.name)
	fmt.Println("+-------------------------+------+")
	fmt.Printf("| Directories              | %d |\n", dir.dirs)
	fmt.Printf("| Symbolic Links           | %d |\n", dir.symLinks)
	fmt.Printf("| Devices                  | %d |\n", dir.devs)
	fmt.Printf("| Sockets                  | %d |\n", dir.sockets)
	fmt.Printf("| Other files              | %d |\n", dir.other)
}

func scanDir(dir string) error {
	dirData := Dir{dir, 0, 0, 0, 0, 0}

	var analyzeDir = func(path string, info os.FileInfo, e error) error {
		if e != nil {
			fmt.Println("Error \"%v\" encountered at <%q>\n", e, path)
			return e
		}
		
		if info.IsDir() {
			dirData.dirs++;
		} else if info.Mode() & os.ModeSymlink != 0 {
			dirData.symLinks++;
		} else if info.Mode() & os.ModeDevice != 0 {
			dirData.devs++;
		} else if info.Mode() & os.ModeSocket != 0 {
			dirData.sockets++;
		} else {
			dirData.other++;
		}
		return nil
	}
	
	e := filepath.Walk(dir, analyzeDir)
	if e != nil {
		fmt.Printf("Error \"%v\" while scanning the path provided: <%q>\n", e, dir)
		return e
	}

	printPathData(dirData)
	return nil
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	scanDir(os.Args[1])
}