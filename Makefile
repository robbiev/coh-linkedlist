CFLAGS=-Wall -g

all: clean
	gcc $(CFLAGS) intrusive.c -o bin/intrusive

clean:
	rm -rf bin
	mkdir bin
