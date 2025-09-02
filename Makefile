CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -I/opt/homebrew/include/ -L/opt/homebrew/lib -lSDL2
SRC_DIR = src
OBJ_DIR = obj
EXEC = bin/main

all: all run

build: $(OBJ_DIR) $(EXEC)

# make an OBJ dir if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(EXEC): $(OBJ_DIR)/main.o
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)

run:
	./bin/main