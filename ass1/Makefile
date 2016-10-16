CC=clang++
CFLAGS=-c -Wall -Wextra -g3
LIBS=-lGL -lglut -lGLU -lGLEW -lm
LDFLAGS=$(LIBS)
SOURCES=Shaders.cpp tergen.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPEND=.depend
EXECUTABLE=tergen

all: $(SOURCES) $(EXECUTABLE) depend

depend: $(DEPEND)

$(DEPEND): $(SOURCES)
	rm -f $@
	$(CC) $(CFLAGS) -MM $^ > $@;

include .depend

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean run
run: all
	./$(EXECUTABLE)
clean:
	rm $(OBJECTS) $(EXECUTABLE) $(DEPEND)
