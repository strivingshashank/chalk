CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -g

chalk:
	$(CC) $(CFLAGS) src/*.c -o bin/chalk -lm

clean:
	rm -f bin/chalk

.PHONY: clean