package geometry

import "math"

type Point struct{ x, y float64 }

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

func Distance(p, q Point) float64 {
	return math.Hypot(q.X-p.X, q.Y-p.Y)
}

func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X-p.X, q.Y-p.Y)
}

type Path []Point

func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			fmt.Printf(" %.4f +", path[i-1].Distance(path[i]))
			sum += path[i-1].Distance(path[i])
		}
	}
	fmt.Printf(" %.4f + ", path[len(path)-1].Distance(path[0]))
	sum += path[len(path)-1].Distance(path[0])
	fmt.Print(" = ")
	return sum
}

