CFLAGS=-Wall -g
BIN_DIR=bin

all: clean
	gcc $(CFLAGS) intrusive.c -o $(BIN_DIR)/intrusive

clean:
	rm -rf $(BIN_DIR)
	mkdir $(BIN_DIR)
