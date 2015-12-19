TARGET = index
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall
LIBSA = libix.a

default: $(TARGET)
all: lib default

lib: $(LIBSA) $(HEADERS)

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBSA): hashtable.o tokenizer.o
	ar -rv $(LIBSA) hashtable.o tokenizer.o

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET) $(LIBSA)
	-rm -f *~