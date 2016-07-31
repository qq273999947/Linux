ROOT_PATH=$(shell pwd)
BIN=httpd
SRC=httpd.c
OBJ=$(SRC:.c=.o)
CC=gcc
FLAGS=-D_DEBUG_
LDFLAGS=-lpthread

$(BIN):$(SRC)
	$(CC) -o $@ $^ $(LDFLAGS)
%.o:%.c
	$(CC) -c $< $(FLAGS) $(LDFLAGS)

.PHONY:clean
clean:
	rm -f *.o httpd

.PHONY:debug
debug:
	@echo $(ROOT_PATH)
	@echo $(BIN)
	@echo $(SRC)
	@echo $(OBJ)
