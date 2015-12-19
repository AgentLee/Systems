TARGET = sl
LIBS = -lm
CC = gcc
CFLAGS =-Wall -g

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@


clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f *~
