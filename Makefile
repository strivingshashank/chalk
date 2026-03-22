CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -I include -lm
TARGET   = bin/chalk
TEST     = bin/test
SRC      = $(wildcard src/*.c)
OBJ      = $(patsubst src/%.c, obj/%.o, $(SRC))
TEST_OBJ = $(filter-out obj/main.o, $(OBJ))

$(TARGET): $(OBJ) | bin
	@$(CC) $(CFLAGS) -o $@ $^

test: $(TEST)
	@./$(TEST)

$(TEST): tests/test.c $(TEST_OBJ) | bin
	@$(CC) $(CFLAGS) -o $@ $^

obj/%.o: src/%.c | obj
	@$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	@./$(TARGET)

bin:
	@mkdir -p bin

obj:
	@mkdir -p obj

clean:
	@rm -f $(OBJ) $(TARGET) $(TEST)

.PHONY: clean test run

