CFLAGS=-Wall -g
BIN_DIR=bin

all: clean
	gcc -c $(CFLAGS) intrusive.c -o $(BIN_DIR)/intrusive.o
	gcc $(CFLAGS) test_intrusive.c bin/intrusive.o -o $(BIN_DIR)/test_intrusive

clean:
	rm -rf $(BIN_DIR)
	mkdir $(BIN_DIR)
