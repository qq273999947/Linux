.PHONY:all
all:client server
client:client.c
	gcc -o $@ $^ -lpthread
server:server.c
	gcc -o $@ $^ -lpthread
.PHONY:clean
clean:
	rm -f client server
