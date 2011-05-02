SRCDIR=src
INCDIR=include
LIBDIR=lib
SRC=$(wildcard *.cpp)
SRC+=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRC))
FLAGS=-g -Wall -Wl,-rpath,$(LIBDIR)
FLAGS+=-Wl,-rpath,$(LIBDIR)/snappy/.libs/
LFLAGS=-lGL -lGLEW -lsnappy -lSDL -lpthread
LFLAGS+=`libpng-config --cflags` `libpng-config --ldflags`
LFLAGS+=`sdl-config --cflags --libs`

CXXFLAGS=$(FLAGS) -I$(INCDIR) -I$(LIBDIR)/glm/ -L$(LIBDIR) $(LFLAGS)
CXXFLAGS+=-I$(LIBDIR)/snappy/ -L$(LIBDIR)/snappy/.libs/
CXX=g++
.SUFFIXES: .o .cpp

%.o: $(SRCDIR)/%.cpp $(SRCDIR)/gui/%.cpp
	$(CXX) $(CXXFLAGS) -c $<

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm $(OBJS)
	rm main
