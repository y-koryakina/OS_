CC = gcc
CFLAGS = -Wall -Wextra -pthread -std=c11
TARGET = copytree

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

clean:
	rm -f $(TARGET)
