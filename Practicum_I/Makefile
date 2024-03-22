all:main
SRC=src/*.c

main: $(SRC)
	gcc $(SRC) -Isrc/include -g -o main
	rm -rf msg_store

LRU: $(SRC)
	gcc $(SRC) -Isrc/include -g -o main -DLRU=1
	rm -rf msg_store

clean:
	rm -rf main
	rm -rf msg_store