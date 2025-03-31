##############################################################################
#
# Author: Lorenzo Moon	
# Description: Basic Makefile for a C program
# Targets: all (default)
# 		   clean           : remove all generated files
# 		   tar             : create a tarball of the files
# 		   git             : add, commit, and push to git
# 		   debug           : compile with debug flags
# 		   test            : run test-PROGRAM.bash
#
###############################################################################

# PROGRAM_NAME must match the main .c file name (without the .c extension)
PROGRAM = PROGRAM_NAME
CC = cc
LDFLAGS = -lm -lX11
TAR_FILE = ${PROGRAM}.tar.gz
TEST_FILE = test-${PROGRAM}.bash

all: $(PROGRAM)

$(PROGRAM): $(PROGRAM).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(PROGRAM).o: $(PROGRAM).c
	$(CC) $(CFLAGS) -c $<

clean clena claen cls cl:
	rm -f $(PROGRAM) $(PROGRAM).o *~ \#*
	rm -f *.output *.error

tar:
	tar -cvaf ${TAR_FILE} *.[ch] [Mm]akefile

git:
	git add .
	git commit -m "Lazy commit via Make"
	git push

test:
	./$(TEST_FILE)

debug: 
	$(CC) $(CFLAGS) $(DFLAGS) -o $(PROGRAM) $(PROGRAM).c $(LDFLAGS)
