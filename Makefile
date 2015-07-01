SRC = $(wildcard *.c)
CFLAGS=$(shell sdl-config --cflags)
LDFLAGS=$(shell sdl-config --libs) -lm

ttsdl: $(SRC)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
