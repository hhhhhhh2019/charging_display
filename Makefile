CC = gcc -c
LD = gcc


SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)



%.o: %.c
	$(CC) $< -o $@

all: $(OBJECTS)
	$(LD) $^ -o charging_display


clean:
	rm *.o charging_display -rf
