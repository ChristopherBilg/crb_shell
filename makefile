# The final file the can be executed
output: main.o parser.o standard.o
	gcc -o output main.o parser.o standard.o -g

# The intermediary object files to be linked together
main.o: main.c
	gcc -o main.o main.c -c -g

# The intermediary object files to be linked together
parser.o: parser.c
	gcc -o parser.o parser.c -c -g

# The intermediary object files to be linked together
standard.o: standard.c
	gcc -o standard.o standard.c -c -g
