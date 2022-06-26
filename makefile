debug: src/main.c
	gcc -g -Wall -Wextra src/main.c -o game

release:
	gcc -O3 src/main.c game

clean:
	rm -r bin/*.o game
