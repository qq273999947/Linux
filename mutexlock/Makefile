BIN = mutex_lock
SRC = mutex_lock.c
CC = gcc
FLAGS = -lpthread

$(BIN):$(SRC)
	$(CC) -o $@ $^ $(FLAGS)
.PHONY:clean
clean:
	rm -rf $(BIN)
