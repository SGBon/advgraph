CXX=g++
CFLAGS=-c -Wall -Wextra -O2 -std=c++11 -Wno-write-strings
LIBS=-lGL -lglut -lGLU -lGLEW -lm
LDFLAGS=$(LIBS)
SOURCES=Shaders.cpp tergen.cpp terproc.cpp util.cpp gl_helper.cpp\
TerrainVAO.cpp Camera.cpp Texture1D.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPEND=.depend
EXECUTABLE=tergen.exe

all: $(SOURCES) $(EXECUTABLE) depend

depend: $(DEPEND)

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
