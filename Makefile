OBJS = \
	src/api.o \
	src/micro.o \
	src/main.o
BIN = micro4
CFLAGS = -lSDL2 -llua

.PHONY: all clean

all: $(BIN)

clean:
	rm -f $(BIN) $(OBJS)

$(BIN): $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

%.o: %.c
	gcc -o $@ -c $< $(CFLAGS)
