CXX=g++
CFLAGS=-c -Wall -Wextra -g3 -std=c++11 -Wno-write-strings
LIBS=-lGL -lglut -lGLU -lGLEW -lm -lfreeimage
LDFLAGS=$(LIBS)
SOURCES=viewer.cpp Shaders.cpp tiny_obj_loader.cpp VAO.cpp boid.cpp helper.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPEND=.depend
EXECUTABLE=animation.exe

all: $(SOURCES) $(EXECUTABLE) $(DEPEND)

$(DEPEND): $(SOURCES)
	rm -f $@
	$(CXX) $(CFLAGS) -MM $^ > $@;

include .depend

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

%.o : %.cpp
	$(CXX) $(CFLAGS) $< -o $@

.PHONY: clean run
run: all
	./$(EXECUTABLE)
clean:
	rm $(OBJECTS) $(EXECUTABLE) $(DEPEND)
