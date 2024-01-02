all: test

test: low_level.o ui.o main.o
	gcc -o test low_level.o ui.o main.o

low_level.o:
	gcc -c low_level.c -o low_level.o

ui:
	gcc -c ui.c -o ui.o

main:
	gcc -c main.c -o main.o

clean:
	rm -rf low_level.o ui.o main.o test
