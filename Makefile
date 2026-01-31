# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -I/opt/homebrew/include -I.
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

# Target and Sources
TARGET = my_game
SRC = main.c
DEPS = buffer.h editor.c

# Default rule
all: $(TARGET)

# Compile the target
# It depends on both the main.c and your included files
$(TARGET): $(SRC) $(DEPS)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Rule to run the game
run: all
	./$(TARGET)

# Clean up the binary
clean:
	rm -f $(TARGET)
