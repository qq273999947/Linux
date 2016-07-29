.PHONY:all
all:server client

client:client.c
	gcc -o client client.c shm.c
server:server.c
	gcc -o server server.c shm.c
.PHONY:clean
clean:
	rm -f client server

