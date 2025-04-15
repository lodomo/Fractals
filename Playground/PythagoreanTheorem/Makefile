PROGRAM = main
CC = cc
LDFLAGS = -lm -lX11 ./utils/FPT.so

# Source files and objects
CSRC = main.c ./utils/Utils.c
HSRC = main.h ./utils/Utils.h
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

