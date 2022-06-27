debug: src/main.c bin/level.o bin/matrix.o bin/game.o bin/viewport.o
	gcc -g -Wall -Wextra src/main.c bin/matrix.o bin/game.o bin/viewport.o -o game -lSDL2 -lSDL2_image -lm

release:
	gcc -O3 src/main.c game

bin/level.o: level.c
	gcc -g -Wall -Wextra -c level.c -o level.o
bin/matrix.o: matrix.c
	gcc -g -Wall -Wextra -c matrix.c -o matrix.o
bin/game.o: game.c
	gcc -g -Wall -Wextra -c game.c -o game.o
bin/viewport.o: viewport.c
	gcc -g -Wall -Wextra -c viewport.c -o viewport.o
clean:
	rm -r bin/*.o game 
