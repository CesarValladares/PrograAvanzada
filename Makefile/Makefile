OBJECTS = simpleTest.o stringTools.o
DEPENDS = stringTools.h
MAIN = simpleTest

CC = gcc
CFLAGS = -std=gnu99 -Wall -pedantic -g
LDFLAGS = -lm
	
$(MAIN): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c $(DEPENDS)
	$(CC) $< -o $@ -c $(CFLAGS)
	
clean:
	rm -rf $(MAIN) *.o

zip:
	zip $(MAIN).zip	*

.PHONY:	clean zip