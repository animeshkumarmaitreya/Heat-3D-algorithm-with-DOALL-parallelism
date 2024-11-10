# Compiler
CC = gcc

# Compiler flags
CFLAGS = -fopenmp -Wall

# Libraries
LDFLAGS = -lnuma -lm  # Added -lm for the math library

# Source and target
TARGET = heat3d
SRC = heat3d.c

# Default rule to build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Rule to clean build artifacts
clean:
	rm -f $(TARGET)
