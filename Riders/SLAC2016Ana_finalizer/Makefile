#
# Makefile with auto dependencies
#

ROOTINCS      = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
SOFLAGS       = -shared
CPPFLAGS      += -MMD -MP -DNDEBUG -fopenmp $(ROOTINCS)
CFLAGS        += -Wall -fPIC
CXXFLAGS      += -Wall -fPIC

ifdef DEBUG
  CFLAGS        += -ggdb
  CXXFLAGS      += -ggdb
  LDFLAGS       += -ggdb
endif

# --- general rule for C compilation
%.o : %.c
	$(COMPILE.c) -o $@ $<

# --- general rule for C++ compilation
%.o : %.cc
	$(COMPILE.cc) -o $@ $<

#
# define executables and source files
#

EXECUTABLES += RiderAnaApp
CXXSOURCES += RiderAnaApp.cc RiderAna.cc RiderNtuple.cc

RiderAnaApp: RiderAnaApp.o RiderAna.o RiderNtuple.o
	$(LINK.cc) -o $@ $^ $(ROOTLIBS)

#
# global targets
#

.PHONY: all
all: $(EXECUTABLES)

clean:
	$(RM) -f *.o *.d
	$(RM) -f $(EXECUTABLES)

#
# automatic dependencies
#

ifeq "$(MAKECMDGOALS)" ""
  -include $(CXXSOURCES:.cc=.d)
  -include $(CSOURCES:.c=.d)
endif
