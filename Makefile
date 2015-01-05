CC=g++

INCLUDE=-I.

SOURCES=$(SRCDIR)/*.cpp

SRCDIR=.

.PHONY: all doc clean

all: vocaMaster

vocaMaster: $(SOURCES)
	$(CC) -o $@ $^ $(INCLUDE)

doc:
	doxygen

clean:
	rm vocaMaster

