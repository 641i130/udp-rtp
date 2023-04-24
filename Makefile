# Compiler to use
CC=g++
# Flags to use
CFLAGS=-c -Wall
# Linker flags to use
LDFLAGS=
# Name of the executable
EXECUTABLE=rajio
# Directories for header files and source files
INCLUDEDIR=include
SRCDIR=src
# Names of source files
SOURCES=$(SRCDIR)/client.cpp $(SRCDIR)/server.cpp $(SRCDIR)/main.cpp
# Names of object files
OBJECTS=$(SOURCES:.cpp=.o)

# Build executable
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) $< -o $@

# Remove object files and executable
clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

# Run tests
test:
	# insert commands to run tests here
