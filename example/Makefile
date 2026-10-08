CC = clang
CFLAGS = -std=c99 -Wall -Wextra -pedantic -I..

.PHONY: clean

examples: *
	$(CC) $(CFLAGS) -o square      00-square.c
	$(CC) $(CFLAGS) -o circle      10-circle.c
	$(CC) $(CFLAGS) -o circle-full 15-circle-full.c
	$(CC) $(CFLAGS) -o tree        20-fractal-tree.c

debug: *
	clang -fsanitize=address -ggdb $(CFLAGS) -o square      00-square.c
	clang -fsanitize=address -ggdb $(CFLAGS) -o circle      10-circle.c
	clang -fsanitize=address -ggdb $(CFLAGS) -o circle-full 15-circle-full.c
	clang -fsanitize=address -ggdb $(CFLAGS) -o tree        20-fractal-tree.c


clean:
	-rm square circle tree
