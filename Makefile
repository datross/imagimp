CC      = gcc
CFLAGS  = -Iinclude/ -I../lib_graphique/include -Wall
LDFLAGS = -L../lib_graphique/lib -Wl,-rpath,../lib_graphique/lib -lglimagimp -lglut -lGL -lGLU 
SRC     = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)
OBJ     = $(SRC:src/%.c=lib/%.o)

all: $(OBJ)
	@#echo "OBJ : $(OBJ)"
	$(CC) $^ -o bin/imagimp $(LDFLAGS) 

main.o: $(HEADERS)

lib/%.o : src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f lib/*.o

re : clean all 
