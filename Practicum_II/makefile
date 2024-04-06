all: server client
SRC=src/*.c

server: src/server.c
	gcc src/server.c -Isrc/include -g -o rfserver

client: src/client.c
	gcc src/client.c -Isrc/include -g -o rfs
