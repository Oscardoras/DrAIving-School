debug: src/main.c bin/entity.o bin/game.o bin/level.o bin/matrix.o bin/viewport.o bin/learning.o
	gcc -g -Wall -Wextra src/main.c bin/entity.o bin/game.o bin/level.o bin/matrix.o bin/viewport.o bin/learning.o -o game -lSDL2 -lSDL2_image -lm -lSDL2_ttf

bin/entity.o: src/entity.c
	gcc -g -Wall -Wextra -c src/entity.c -o bin/entity.o
bin/game.o: src/game.c
	gcc -g -Wall -Wextra -c src/game.c -o bin/game.o
bin/level.o: src/level.c
	gcc -g -Wall -Wextra -c src/level.c -o bin/level.o
bin/matrix.o: src/matrix.c
	gcc -g -Wall -Wextra -c src/matrix.c -o bin/matrix.o
bin/viewport.o: src/viewport.c
	gcc -g -Wall -Wextra -c src/viewport.c -o bin/viewport.o
bin/learning.o: src/learning.c
	gcc -g -Wall -Wextra -c src/learning.c -o bin/learning.o

release:
	gcc -O3 src/main.c src/entity.c src/game.c src/level.c src/matrix.c src/viewport.c -o game -lSDL2 -lSDL2_image -lm -lSDL2_ttf
	
clean:
	rm -r bin/*.o game 
