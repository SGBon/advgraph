CC=g++
CFLAGS=-c -Wall -Wextra -g3
LIBS=-lGL -lglut -lGLU -lGLEW -lm
LDFLAGS=$(LIBS)
SOURCES=Shaders.cpp tergen.cpp terproc.cpp util.cpp gl_helper.cpp TerrainVAO.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPEND=.depend
EXECUTABLE=tergen

all: $(SOURCES) $(EXECUTABLE) depend

depend: $(DEPEND)

$(DEPEND): $(SOURCES)
	rm -f $@
	$(CXX) $(CFLAGS) -MM $^ > $@;

include .depend

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CXX) $(CFLAGS) $< -o $@

.PHONY: clean run
run: all
	./$(EXECUTABLE)
clean:
	rm $(OBJECTS) $(EXECUTABLE) $(DEPEND)
