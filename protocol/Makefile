SHELL = /bin/sh
CC    = g++

CFLAGS       = -Wall  -Iinclude
LDLIBS		 = 
TARGET  = protocol
SOURCES = $(shell echo src/*.cpp)
HEADERS = $(shell echo include/*.h)
OBJECTS = $(SOURCES:.c=.o)
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
 
$(foreach OBJ,$(OBJECTS),$(eval $(OBJ)_DEPS = $(shell $(CC) $(CFLAGS) -MM $(OBJ:.o=.cpp) | sed s/.*://)))
%.o: %.c $$($$@_DEPS)
	$(CC) $(CFLAGS) $(LDLIBS) -c -o $@ $<
  
 
.PHONY : all install uninstall clean distclean
