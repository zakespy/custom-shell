CC = gcc
CFLAGS = -Wall
TARGET = shell

all: $(TARGET)

$(TARGET): shell.c
	$(CC) $(CFLAGS) -o $(TARGET) shell.c

run: $(TARGET)
	./$(TARGET)

.PHONY: all run clean