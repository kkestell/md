CC = clang
CFLAGS = -Wall -Werror -O2 -pedantic -std=c11
BIN = md
SRC = main.c
OBJ = $(SRC:.c=.o)
PREFIX = ~/.local/bin

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

.PHONY: clean install

clean:
	rm -f $(BIN) $(OBJ)

install: $(BIN)
	cp $(BIN) $(PREFIX)
