PROGRAM = orbit
CC = cc
LDFLAGS = -lm -lX11

all: $(PROGRAM)

$(PROGRAM): $(PROGRAM).o
	$(CC) -o $@ $^ $(LDFLAGS)

$(PROGRAM).o: $(PROGRAM).c
	$(CC) -c $< -o $@

clean:
	rm -f $(PROGRAM) $(PROGRAM).o *~ \#* *.output *.error
