CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGET = test
SRC = ex5.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
