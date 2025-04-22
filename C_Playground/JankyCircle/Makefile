PROGRAM = main
CC = cc
PATH_TO_FPT = ./srcs/FPT.so
LDFLAGS = -lm -lX11 $(PATH_TO_FPT)

# Source files and objects
CSRC = main.c
HSRC = main.h
OBJ = $(CSRC:.c=.o)

# Default target
all: $(PROGRAM)

# Link program with FPT.so
$(PROGRAM): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile C files to object files
%.o: %.c $(HSRC)
	$(CC) -c $< -o $@

# Run the program with local LD_LIBRARY_PATH
run: $(PROGRAM)
	LD_LIBRARY_PATH=. ./$(PROGRAM)

# Clean up
clean clena lcean claen:
	rm -f $(PROGRAM) $(OBJ) *~ \#* *.output *.error

