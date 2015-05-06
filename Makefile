CC = gcc

SDIR = src
HDIR = headers
LDIR = libs/lib
IDIR = libs/include
ODIR = obj

_OBJ = main.o core.o input.o jumping_logic.o jumping_draw.o geometry.o procedural.o new_geometry.o objloader.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

CFLAGS = -I$(IDIR) -I$(HDIR) -L$(LDIR) -Wall -Wextra -pedantic -std=c99
LIBS = -lm -lglut -lGLU -lGL libs/lib/libSOIL.a

all: frogger

debug: CFLAGS += -g
debug: all

$(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

frogger: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	-rm -r $(ODIR) frogger
