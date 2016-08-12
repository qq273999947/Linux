ROOT_PATH=$(shell pwd)
CC=g++
INCLUDE_PATH=-I$(ROOT_PATH)/../lib/lib/INCLUDE_PATH

cgi_sql:cgi_sql.c
	$(CC) $@ $^ $(INCLUDE_PATH)

.PHONY:clean
clean:
	rm -f cgi_sql
