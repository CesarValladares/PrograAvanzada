# César Armando Valladares
# A01023506

OBJECTS = main.o ppm.o
DEPENDS = ppm.h
MAIN = main

CC = gcc
CFLAGS = -std=gnu99 -Wall -pedantic -g
LDFLAGS = -lm
	
$(MAIN): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c $(DEPENDS)
	$(CC) $< -o $@ -c $(CFLAGS)
	
clean:
	rm -rf $(MAIN) *.o
