TARGETS =  $(patsubst %.cc,%,$(wildcard *.cc))
CPPFLAGS += -std=c++17 -Wall

program_INCLUDE_DIRS :=
program_LIBRARY_DIRS :=
program_LIBRARIES := pthread

CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all clean distclean

all: $(TARGETS)

$(TARGETS): %: %.cc
	    $(LINK.cc) -o $@.out $< 

clean:
	$(RM) *.out *.o
