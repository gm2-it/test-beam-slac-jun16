AR            = g++
CXX           = g++ 
ROOTINCS      = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
CXXFLAGS      = -g -Wall -fPIC $(ROOTINCS)
SOFLAGS       = -shared

all: NaDaqAnaApp

NaDaqAnaAppObjs = NaDaqAnaApp.o NaDaqAna.o EventCollection.o

NaDaqAnaApp: $(NaDaqAnaAppObjs)
	$(CXX) -o $@ $(NaDaqAnaAppObjs) $(ROOTLIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	/bin/rm -f *.o $(APPNAME) *~
