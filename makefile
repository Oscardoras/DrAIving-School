debug: src/main.c bin/level.o bin/matrix.o bin/game.o bin/viewport.o
	gcc -g -Wall -Wextra src/main.c bin/matrix.o bin/game.o bin/viewport.o -o game -lSDL2 -lSDL2_image -lm

release:
	gcc -O3 src/main.c game

bin/level.o: src/level.c
	gcc -g -Wall -Wextra -c src/level.c -o level.o
bin/matrix.o: src/matrix.c
	gcc -g -Wall -Wextra -c src/matrix.c -o matrix.o
bin/game.o: src/game.c
	gcc -g -Wall -Wextra -c src/game.c -o game.o
bin/viewport.o: src/viewport.c
	gcc -g -Wall -Wextra -c src/viewport.c -o viewport.o
clean:
	rm -r bin/*.o game 
