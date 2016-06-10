APPNAME = NaDaqAnaApp

OBJS = NaDaqAnaApp.o NaDaqAna.o EventCollection.o

AR            = g++
CXX           = g++ 
ROOTINCS      = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
CXXFLAGS      = -g -Wall -fPIC $(ROOTINCS)
SOFLAGS       = -shared

all: $(OBJS)
	$(CXX) -o $(APPNAME) $(OBJS) $(ROOTLIBS)

%.o:%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	/bin/rm -f *.o $(APPNAME) *~
