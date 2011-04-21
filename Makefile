SRCDIR=src
INCDIR=include
LIBDIR=lib
SRC=$(wildcard *.cpp)
SRC+=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRC))
FLAGS=-g -Wall -Wl,-rpath,$(LIBDIR)/snappy/.libs/
FLAGS+=-Wl,-rpath,$(LIBDIR)/sfml/build/lib/
FLAGS+=-Wl,-rpath,$(LIBDIR)/FreeImage/Dist/
LFLAGS=-lGL -lGLU -lGLEW -lsnappy -lsfml-window -lsfml-graphics -lglut -lfreeimage

CXXFLAGS=$(FLAGS) -I$(INCDIR) -I$(LIBDIR) -I$(LIBDIR)/glm/ 
CXXFLAGS+=-I$(LIBDIR)/snappy/ -I$(LIBDIR)/snappy/.libs -L$(LIBDIR)/snappy/.libs 
CXXFLAGS+=-I$(LIBDIR)/sfml/include/ -L$(LIBDIR)/sfml/build/lib $(LFLAGS)
CXXFLAGS+=-I$(LIBDIR)/FreeImage/Dist/ -L$(LIBDIR)/FreeImage/Dist/
CXX=g++
.SUFFIXES: .o .cpp

%.o: $(SRCDIR)/%.cpp $(SRCDIR)/gui/%.cpp
	$(CXX) $(CXXFLAGS) -c $<

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm $(OBJS)
	rm main
