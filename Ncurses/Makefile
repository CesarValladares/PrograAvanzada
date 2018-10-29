OBJECTS = demo.c
#DEPENDS = stringTools.h
MAIN = demo

CC = gcc
CFLAGS = -std=gnu99 -Wall -pedantic -g
LDFLAGS = -lncurses -lpthread
	
$(MAIN): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c $(DEPENDS)
	$(CC) $< -o $@ -c $(CFLAGS)
	
clean:
	rm -rf $(MAIN) *.o