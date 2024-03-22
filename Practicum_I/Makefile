all:main clear_store
SRC=src/*.c

main: $(SRC)
	gcc $(SRC) -Isrc/include -g -o main

clear_store:
	rm -rf msg_store

LRU: $(SRC)
	gcc $(SRC) -Isrc/include -g -o main -DLRU=1