CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -L/usr/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lopenblas
INCLUDES = -I/usr/include/SDL2

SRC = main.c b_spline.c gui.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = program

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)

