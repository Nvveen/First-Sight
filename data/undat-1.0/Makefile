LIBDIR=../../lib
FLAGS=-Wall -Wl,-rpath,$(LIBDIR)/snappy/.libs/ -g
FLAGS+=-Wl,-rpath,$(LIBDIR)/sfml/build/lib/
LFLAGS=-lsnappy -lsfml-graphics

CXXFLAGS=$(FLAGS) -L$(LIBDIR)/snappy/.libs -I$(LIBDIR)/snappy/ -I$(LIBDIR)/snappy/.libs 
CXXFLAGS+=-I$(LIBDIR)/sfml/include/ -L$(LIBDIR)/sfml/build/lib -I../../include/ $(LFLAGS)
CXX=g++

undat: Undat.o
	$(CXX) $(CXXFLAGS) -o undat Undat.o

Undat.o: Undat.cpp
	$(CXX) $(CXXFLAGS) Undat.cpp -c

clean:
	rm *.o
	rm undat
