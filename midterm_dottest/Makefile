CC = aarch64-linux-gnu-gcc

CFLAGS	=
LDFLAGS = -static

default: midterm_dottest


midterm_dottest: midterm_dottest.o
	$(CC) $(LDFLAGS) -o midterm_dottest midterm_dottest.o

midterm_dottest.o: midterm_dottest.c
	$(CC) $(CFLAGS) -o midterm_dottest.o -c midterm_dottest.c

clean:
	rm midterm_dottest.o midterm_dottest
