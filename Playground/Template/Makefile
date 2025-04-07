PROGRAM = main
CC = cc
LDFLAGS = -lm -lX11 ./FPT.so

# Source files and objects
CSRC = main.c LDM_Utils.c
HSRC = main.h LDM_Utils.h
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
clean:
	rm -f $(PROGRAM) $(OBJ) *~ \#* *.output *.error

