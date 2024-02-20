all:main
SRC=src/*.c

main: $(SRC)
	gcc $(SRC) -Isrc/include -g -o main

test: $(SRC)
	gcc $(SRC) -DTEST=1 -Isrc/include -g -o test