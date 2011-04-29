SRCDIR=src
INCDIR=include
LIBDIR=lib
SRC=$(wildcard *.cpp)
SRC+=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRC))
FLAGS=-g -Wall -Wl,-rpath,$(LIBDIR)/snappy/.libs/
FLAGS+=-Wl,-rpath,$(LIBDIR)/boost/stage/libs/
FLAGS+=-Wl,-rpath,$(LIBDIR)/SDL/build/.libs/
LFLAGS=-lGL -lGLU -lGLEW -lsnappy -lglut -lboost_signals -lSDL -lpthread
LFLAGS+=`libpng-config --cflags` `libpng-config --ldflags`

CXXFLAGS=$(FLAGS) -I$(INCDIR) -I$(LIBDIR) -I$(LIBDIR)/glm/ 
CXXFLAGS+=-I$(LIBDIR)/snappy/ -I$(LIBDIR)/snappy/.libs -L$(LIBDIR)/snappy/.libs 
CXXFLAGS+=-L$(LIBDIR)/SDL/build/.libs/ -I$(LIBDIR)/SDL/include/
CXXFLAGS+=-I$(LIBDIR)/boost/ -L$(LIBDIR)/boost/stage/libs/ $(LFLAGS)
CXX=g++
.SUFFIXES: .o .cpp

%.o: $(SRCDIR)/%.cpp $(SRCDIR)/gui/%.cpp
	$(CXX) $(CXXFLAGS) -c $<

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm $(OBJS)
	rm main
