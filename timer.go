// fraq <code@fraq.io>

package main

import (
	"bufio"
	"flag"
	"fmt"
	"io/ioutil"
	"strings"
	"time"
)

func main() {
	var corpus string
	flag.StringVar(&corpus, "c", "corpus.txt", "Location of corpus to parse")
	flag.Parse()

	start := time.Now()
	data, err := ioutil.ReadFile(corpus)
	if err != nil {
		panic(err)
	}
	scanner := bufio.NewScanner(strings.NewReader(string(data)))
	scanner.Split(bufio.ScanWords)
	max := 0
	longestWord := ""

	scan := time.Now()
	for scanner.Scan() {
		tkLen := len(scanner.Text())
		if tkLen >= max {
			max = tkLen
			longestWord = scanner.Text()
		}

	}

	end := time.Now()

	fmt.Printf("Longest word is %d characters\n", max)
	fmt.Printf("Longest word is %s\n", longestWord)

	fmt.Println("Total time:\t\t", end.Sub(start).Nanoseconds()/1000000)
	fmt.Println("Scan time:\t\t", end.Sub(scan).Nanoseconds()/1000000)
}
