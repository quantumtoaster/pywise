SRC = $(wildcard source/*.c)
OBJ = $(notdir $(SRC:.c=.o))
LIB = libpairwise.a

CFLAGS = -Wall -Iinclude -lm -pthread -fPIC -Ofast

print-%: ; @echo $* = $($*)

all: $(LIB)

$(LIB): $(OBJ)
	ar rc $@ $(OBJ)
	ranlib $@

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

.PHONY: clean
clean:
	rm --force $(OBJ) $(LIB)

