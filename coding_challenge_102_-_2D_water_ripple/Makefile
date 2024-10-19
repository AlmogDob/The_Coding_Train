build: simulator.c
	gcc -Wall -Wextra ./simulator.c -lSDL2 -lSDL2_ttf -lm -o simulator

run:
	./simulator

clean:
	rm simulator

test:
	gcc -Wall -Wextra ./test.c -lSDL2 -lSDL2_ttf -lm -o test
	./test
	rm test