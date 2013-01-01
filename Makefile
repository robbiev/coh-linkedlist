CC=clang
CFLAGS=-Wall -g -std=c99
BIN_DIR=bin

all: clean
	$(CC) -c $(CFLAGS) intrusive.c -o $(BIN_DIR)/intrusive.o
	$(CC) $(CFLAGS) test_intrusive.c $(BIN_DIR)/intrusive.o -o $(BIN_DIR)/test_intrusive

clean:
	rm -rf $(BIN_DIR)
	mkdir $(BIN_DIR)
