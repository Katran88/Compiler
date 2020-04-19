# compiler-course-project-

This is a compiler from my own HDV2019 language to an Assembly language. In the file in.txt here is an example of the source code written in HDV2019, which demonstrates all its features:

	- 4 data types: ubyte, int, logic, string
	- cycles
	- if block with multi-conditions
	- preprocessor directive _connect
	- global and local variables, variable scopes
	- automatic conversion between types(except for a string, it has the inttostr(int var) / strtoint(string var) functions )
	- function for getting a random number from range random(int from, int to)
	- functions for getting current date getDate () and time getTime()
	- string concatenation, bitwise and algebraic operations
	- output to the console: cprint, cprintl
