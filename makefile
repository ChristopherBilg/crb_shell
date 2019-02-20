output: main.o parser.o standard.o
	gcc -o output main.o parser.o standard.o -g

main.o: main.c
	gcc -o main.o main.c -c -g

parser.o: parser.c
	gcc -o parser.o parser.c -c -g

standard.o: standard.c
	gcc -o standard.o standard.c -c -g
