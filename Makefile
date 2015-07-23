SHELL = /bin/sh
CC    = g++

CPPFLAGS       = -g -Wall -Iinclude
CFLAGS       = $(CPPFLAGS) 
LDLIBS		 = -lutil 
TARGET  = terminal protocol options
SOURCES = $(shell echo src/*.cpp)
HEADERS = $(shell echo include/*.h)
OBJECTS = $(SOURCES:.cpp=.o)
DEBUGFLAGS = -g
 
all: $(TARGET)
 
terminal: $(OBJECTS) main/terminal.o $(HEADERS)
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $(OBJECTS) main/$@.o

protocol: $(OBJECTS) main/protocol.o $(HEADERS)
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $(OBJECTS) main/$@.o 

options: $(OBJECTS) main/options.o $(HEADERS)
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $(OBJECTS) main/$@.o 


clean:
	-rm -f $(OBJECTS)
	-rm -f gmon.out
	-rm -f main/*.o
 
distclean: clean
	-rm -f $(TARGET)
 
 
.SECONDEXPANSION:
 
$(foreach OBJ,$(OBJECTS),$(eval $(OBJ)_DEPS = $(shell $(CC) $(CFLAGS) -MM $(OBJ:.o=.cpp) | sed s/.*://)))
%.o: %.cpp $$($$@_DEPS)
	$(CC) $(CFLAGS) $(LDLIBS) -c -o $@ $<
  
 
.PHONY : all install uninstall clean distclean
