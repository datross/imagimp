DIR_OBJ = obj
DIR_INCLUDE = include
DIR_SRC = src
DIR_GLIMAGIMP = lib/glimagimp
DIR_BIN = bin

CC      = gcc
CFLAGS  = -I$(DIR_INCLUDE) -I$(DIR_GLIMAGIMP)/include -Wall -std=c99
LDFLAGS = -L$(DIR_GLIMAGIMP)/lib -Wl,-rpath,$(DIR_GLIMAGIMP)/lib -lglimagimp -lglut -lGL -lGLU 

SRC     = $(wildcard $(DIR_SRC)/*.c)
HEADERS = $(wildcard $(DIR_INCLUDE)/*.h)
OBJ     = $(SRC:$(DIR_SRC)/%.c=$(DIR_OBJ)/%.o)

all: libglimagimp $(OBJ)
	$(CC) $(filter-out $<, $^)  -o $(DIR_BIN)/imagimp $(LDFLAGS) 

main.o: $(HEADERS)

$(DIR_OBJ)/%.o : $(DIR_SRC)/%.c $(HEADERS)
	$(CC) -o $@ -c $< $(CFLAGS)

libglimagimp:
	cd $(DIR_GLIMAGIMP) && $(MAKE) && cd ../..

.PHONY: clean

clean:
	rm -f $(DIR_OBJ)/*.o

re : clean all 
