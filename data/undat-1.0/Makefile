LIBDIR=../../lib
FLAGS=-Wall -Wl,-rpath,$(LIBDIR)/snappy/.libs/ -g
LFLAGS=-lsnappy

CXXFLAGS=$(FLAGS) -L$(LIBDIR)/snappy/.libs -I$(LIBDIR)/snappy/ -I$(LIBDIR)/snappy/.libs -I../../include/ $(LFLAGS)
CXXFLAGS+=`libpng-config --cflags` `libpng-config --ldflags`
CXX=g++

undat: Undat.o
	$(CXX) $(CXXFLAGS) -o undat Undat.o

Undat.o: Undat.cpp
	$(CXX) $(CXXFLAGS) Undat.cpp -c

clean:
	rm *.o
	rm undat
