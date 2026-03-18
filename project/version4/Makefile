CC = gcc
CFLAGS = -Wall -g
TARGET = memory_allocator
SRCS = memory_allocator.c main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o
