CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGET = ex7
SRC = ex7.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
