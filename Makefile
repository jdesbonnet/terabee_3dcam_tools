CC=gcc
DEPS=-lrt -lm
SOURCES= terabee_3dcam.c
EXECUTABLE= terabee_3dcam

$(EXECUTABLE): $(SOURCES) $(DEPS)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)
