package main

import (
	"flag"
	"fmt"
	"strings"
)

var arg1 = flag.String("arg1", "Default_arg1", "Argument 1 is string!")
var arg2 = flag.Int("arg2", 100, "Argument 2 is integer")
var arg3 = flag.Bool("arg3", false, "Argument 3 is boolean")

type interval []string

func (i *interval) String() string {
	return fmt.Sprint(*i)
}

func (i *interval) Set(value string) error {
	for _, dt := range strings.Split(value, ",") {
		*i = append(*i, dt)
	}

	return nil
}

var intervalFlag interval

func init() {
	flag.Var(&intervalFlag, "strings", "Some strings seperated with commas")
}

func main() {
	flag.Parse()
	fmt.Printf("Arg 1 : %s\nArg2 : %d\nArg3 : %t\n",
		*arg1, *arg2, *arg3)
	fmt.Println(intervalFlag)
}
