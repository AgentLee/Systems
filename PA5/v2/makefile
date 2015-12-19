TARGET = orders
LIBS = -lm
CC = gcc
CFLAGS =-Wall -g
LIBSA = libos.a

default: $(TARGET)
all: lib default

lib: $(LIBSA) $(HEADERS)

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(LIBSA): bookorder.o
	ar -rv $(LIBSA) bookorder.o

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@ -lpthread

clean:
	-rm -f *.o
	-rm -f $(TARGET) $(LIBSA)
	-rm -f *~
