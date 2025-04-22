PROGRAM = n_lines
CC = cc
LDFLAGS = -lm -lX11
TAR_FILE = ${PROGRAM}.tar.gz
TEST_FILE = test-${PROGRAM}.bash

all: $(PROGRAM)

$(PROGRAM): $(PROGRAM).o
	$(CC) -o $@ $^ $(LDFLAGS)

$(PROGRAM).o: $(PROGRAM).c
	$(CC) -c $<

clean:
	rm -f $(PROGRAM) $(PROGRAM).o *~ \#* *.output *.error

tar:
	tar -cvaf ${TAR_FILE} *.[ch] [Mm]akefile

git:
	git add .
	git commit -m "Lazy commit via Make"
	git push

test:
	./$(TEST_FILE)
