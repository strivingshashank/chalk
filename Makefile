CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -I include -g
LDFLAGS  = -lm
TARGET   = bin/chalk
TEST     = bin/test
SRC      = $(wildcard src/*.c)
OBJ      = $(patsubst src/%.c, obj/%.o, $(SRC))
TEST_OBJ = $(filter-out obj/main.o, $(OBJ))

$(TARGET): $(OBJ) | bin
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test: $(TEST)
	@./$(TEST)

$(TEST): tests/test.c $(TEST_OBJ) | bin
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c | obj
	@$(CC) $(CFLAGS) -c -o $@ $^

bin:
	@mkdir -p bin

obj:
	@mkdir -p obj

clean:
	@rm -f $(OBJ) $(TARGET) $(TEST)

.PHONY: clean test run