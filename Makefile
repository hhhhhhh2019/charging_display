FLAGS = -Os -s -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-stack-protector -fomit-frame-pointer -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-math-errno -fno-unroll-loops -fmerge-all-constants -ffast-math -fno-ident

CC = gcc -c -I ./include $(FLAGS)
LD = gcc $(FLAGS)


SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)



%.o: %.c
	$(CC) $< -o $@

all: $(OBJECTS)
	$(LD) $^ -o charging_display


clean:
	rm *.o charging_display -rf
