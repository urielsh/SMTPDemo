#
# Generic Makefile to compile an executable
# 
#

# our compiler
CC=gcc

# compile arguments
CFLAGS+=-c -g -Wall

# linker flags
LDFLAGS+=

# libraries
LIBS+=-lncurses -lm

# our source files
SOURCES= Csmtp.c
# a macro to define the objects from sources
OBJECTS=$(SOURCES:.c=.o)

# executable name
EXECUTABLE=bf

$(EXECUTABLE): $(OBJECTS)
	@echo "Building target" $@ "..."
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

# a rule for generating object files given their c files
.c.o:
	@echo "Compiling" $< "..."
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *s *o $(EXECUTABLE)

.PHONY: all clean
