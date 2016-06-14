#
# Makefile with auto dependencies
#

# --- setup automatic dependencies
DEPDIR 	      := .
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS      = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d.tmp

ROOTINCS      := $(shell root-config --cflags)
ROOTLIBS      := $(shell root-config --libs)

CFLAGS        += -Wall -fPIC -fopenmp -DNDEBUG $(ROOTINCS)
CXXFLAGS      += -Wall -fPIC -fopenmp -DNDEBUG $(ROOTINCS)
SOFLAGS       := -shared

ifdef DEBUG
  CFLAGS        += -ggdb
  CXXFLAGS      += -ggdb
  LDFLAGS       += -ggdb
endif

# --- add dependency generation on compilation
COMPILE.c += $(DEPFLAGS)
COMPILE.cc += $(DEPFLAGS)
# --- produce .d files only if compilation was successful
COMPILE.dep = mv -f $(DEPDIR)/$*.d.tmp $(DEPDIR)/$*.d

# --- general rule for C compilation
%.o : %.c
%.o : %.c $(DEPDIR)/%.d
	$(COMPILE.c) -o $@ $<
	$(COMPILE.dep)

# --- general rule for C++ compilation
%.o : %.cc
%.o : %.cc $(DEPDIR)/%.d
	$(COMPILE.cc) -o $@ $<
	$(COMPILE.dep)

# --- prevent failure if a dependency file does not exist
$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

#
# define executables and source files
#

EXECUTABLES += RiderAnaApp
CXXSOURCES += RiderAnaApp.cc RiderAna.cc EventCollection.cc

RiderAnaApp: RiderAnaApp.o RiderAna.o EventCollection.o
	$(LINK.cc) -o $@ $^ $(ROOTLIBS)

#
# global targets
#

.PHONY: all
all: $(EXECUTABLES)

clean:
	$(RM) $(EXECUTABLES) *.o *.d *.d.tmp

#
# automatic dependencies
#

# --- if statement prevents non required dependency generation on make clean
ifeq "$(MAKECMDGOALS)" ""
  -include $(patsubst %, $(DEPDIR)/%.d, $(basename $(CSOURCES) $(CXXSOURCES)))
endif
