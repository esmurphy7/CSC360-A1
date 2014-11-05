Specs - https://github.com/esmurphy7/CSC360-A1/blob/master/p1.pdf

Project Features
Core Functionality:
1) execute internal commands such as "ls" and "ping"
2) change working directory
3) execute internal commands in the background
4) list all active background processes
Extra Functionality:
5) users can kill a specific background process by typing "kill <pid>"
6) users can quit the interpreter by typing "q" (doing so will kill all background processes)

How to Compile:
type "make" to create an executable "shell.exe"

How to Run:
type "./shell.exe" to run the code

File Breakdown:
makefile
	- links source with headers
	- creates object files from source
	- creates a single executable "shell.exe"
shell.c
	- Manages the main user input loop, executes commands, and handles background processes
	- Utilizes linkedlist.c to store process information
	- Delegates parsing and standard string utilities to parser.c and utilities.c

parser.c
	- Parses the raw input from the user

utilities.c
	- modularizes common string functions

linkedlist.c
	- list structure to store information on background processes such as pid and execution path