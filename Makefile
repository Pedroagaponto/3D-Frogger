CC = gcc

SDIR = src
IDIR = headers
ODIR = obj

_OBJ = main.o core.o input.o jumping_logic.o jumping_draw.o geometry.o procedural.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

CFLAGS = -I$(IDIR) -Wall -Wextra -pedantic -g -std=c99
LIBS = -lm -lGL -lGLU -lglut

all: frogger

$(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

frogger: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -r $(ODIR) frogger
