package main

import "golang.org/x/tour/pic"

func Picture(dx, dy int) [][]uint8 {
	var arr [][]uint8

	arr = make([][]uint8, dy)

	for i := 0; i < len(arr); i++ {
		arr[i] = make([]uint8, dx)

		for j := 0; j < dx; j++ {
			arr[i][j] = uint8(i) * (uint8(j)/2)
		}
	}

	return arr
}

func main() {
	pic.Show(Picture)
}
