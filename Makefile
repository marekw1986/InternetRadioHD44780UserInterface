CFLAGS = -DPCDEBUG

all: test

test: low_level.o ui.o scrollable_list.o main_list.o main.o
	gcc $(CFLAGS) -o test low_level.o ui.o scrollable_list.o main_list.o main.o

low_level.o:
	gcc $(CFLAGS) -c low_level.c -o low_level.o

ui:
	gcc $(CFLAGS) -c ui.c -o ui.o
	
main_list:
	gcc $(CFLAGS) -c main_list.c	-o main_list.o
	
scrollable_list:
	gcc $(CFLAGS) -c scrollable_list.c -o scrollable_list.o

main:
	gcc $(CFLAGS) -c main.c -o main.o

clean:
	rm -rf *.o test
