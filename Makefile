DIR_OBJ = obj/
DIR_INCLUDE = include/
DIR_SRC = src/
DIR_IMAGIMP = lib/glimagimp/
DIR_BIN = bin/

CC      = gcc
CFLAGS  = -I$(DIR_INCLUDE) -I$(DIR_IMAGIMP)/include -Wall
LDFLAGS = -L$(DIR_IMAGIMP)/lib -Wl,-rpath,$(DIR_IMAGIMP)/lib -lglimagimp -lglut -lGL -lGLU 

SRC     = $(wildcard $(DIR_SRC)*.c)
HEADERS = $(wildcard $(DIR_INCLUDE)*.h)
OBJ     = $(SRC:$(DIR_SRC)%.c=$(DIR_OBJ)%.o)

all: libglimagimp $(OBJ)
	$(CC) $(filter-out $<, $^)  -o $(DIR_BIN)/imagimp $(LDFLAGS) 

main.o: $(HEADERS)

$(DIR_OBJ)%.o : $(DIR_SRC)%.c
	$(CC) -o $@ -c $< $(CFLAGS)

libglimagimp:
	cd lib/glimagimp/ && $(MAKE) 

.PHONY: clean

clean:
	rm -f $(DIR_OBJ)*.o

re : clean all 
