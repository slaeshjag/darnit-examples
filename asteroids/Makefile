CFLAGS	+=	-O0 -g -Wall
LDFLAGS	+=	-ldarnit
SRC	+=	src/asteroids.c src/collision.c
BIN	+=	bin/asteroids

default:
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)
	make -C res/
	cat res/data.ldi >> $(BIN)

clean:
	rm -Rf bin
	make -C res/ clean
