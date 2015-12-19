TARGET = sl
LIBS = -lm
CC = gcc
CFLAGS =-Wall -g
LIBSA = libsl.a

default: $(TARGET)
all: lib default

lib: $(LIBSA) $(HEADERS)

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBSA): sorted-list.o
	ar -rv $(LIBSA) sorted-list.o

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET) $(LIBSA)
	-rm -f *~