SHELL = /bin/sh
CC    = g++

CPPFLAGS       = -Wall  -Iinclude
CFLAGS		= $(CPPFLAGS)
LDLIBS		 = 
TARGET  = protocol
SOURCES = $(shell echo src/*.cpp) test/sample.cpp
HEADERS = $(shell echo include/*.h)
OBJECTS = $(SOURCES:.cpp=.o)
DEBUGFLAGS = -g
 
all: $(TARGET)
 
$(TARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(LDLIBS) -o $(TARGET) $(OBJECTS)
 
clean:
	-rm -f $(OBJECTS)
	-rm -f gmon.out
 
distclean: clean
	-rm -f $(TARGET)
 
 
.SECONDEXPANSION:
 
$(foreach OBJ,$(OBJECTS),$(eval $(OBJ)_DEPS = $(shell $(CC) $(CFLAGS)  -MM $(OBJ:.o=.cpp) | sed s/.*://)))
%.o: %.cpp $$($$@_DEPS)
	$(CC) $(CFLAGS) $(LDLIBS) -c -o $@ $<
  
 
.PHONY : all install uninstall clean distclean
