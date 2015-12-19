TARGET1 = index 
TARGET2 = search 
CC = gcc
CFLAGS = -g -Wall

default: $(TARGET1) $(TARGET2)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

OBJ1 = hashtable.o tokenizer.o indexer.o 
OBJ2 = hashtable.o tokenizer.o search.o utils.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


$(TARGET1): $(OBJ1)
	$(CC) $(OBJ1) $(CFLAGS) -o index

$(TARGET2): $(OBJ2)
	$(CC) $(OBJ2) $(CFLAGS) -o search

clean:
	-rm -f *.o
	-rm -f $(TARGET1) $(TARGET2) $(LIBSA)
	-rm -f *~
