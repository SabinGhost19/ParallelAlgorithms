CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGET = ex8
SRC = ex8.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
