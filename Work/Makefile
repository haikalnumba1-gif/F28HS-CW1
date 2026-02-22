# Options to pass to the C compiler
CC = gcc
CFLAGS = -std=c99 -O2 -Wall -pedantic -g

# List of programs to build
TARGETS = process

# Default rule, built if you just say "make"
all: $(TARGETS)

# Build a program
$(TARGETS): $(TARGETS).c
	$(CC) $(CFLAGS) -o $@ $<

# Remove built programs and temporary files
clean:
	rm -f $(TARGETS) *~
