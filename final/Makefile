CXX=g++
CFLAGS=-c -Wall -Wextra -g -std=c++11 -Wno-write-strings
LIBS=-lGL -lglut -lGLU -lGLEW -lm
LDFLAGS=$(LIBS)
SOURCES=viewer.cpp Shaders.cpp VAO.cpp tiny_obj_loader.cpp lsystem.cpp Camera.cpp glm_helpers.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPEND=.depend
EXECUTABLE=lsys.exe

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
