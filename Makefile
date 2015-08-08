SHELL = /bin/sh
CC    = g++
OLDFLAGS	= -std=c++11
CPPFLAGS       = -g -Wall -Iinclude 
CFLAGS       = $(CPPFLAGS) 
LDLIBS		 = -lutil -lpam
TARGET  = terminal protocol options tcpclient tcpservertty parameters daemon authent
SOURCES = $(shell echo src/*.cpp)
HEADERS = $(shell echo include/*.h)
OBJECTS = $(SOURCES:.cpp=.o)
MAIN	= $(shell echo main/*.cpp)
MAINOBJ	= $(MAIN:.cpp=.o)
DEBUGFLAGS = -g
 
all: $(TARGET)
 
%:   main/%.o $(OBJECTS)  $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $< $(LDLIBS)

clean:
	-echo "clean"
	-rm -f $(OBJECTS)
	-rm -f gmon.out
	-rm -f main/*.o
 
distclean: clean
	-rm -f $(TARGET)
 
gitadd:
	-git add src/*.cpp
	-git add include/*.h
	-git add main/*.cpp
	-git add Makefile
 
.SECONDARY: $(OBJECTS) $(MAINOBJ)
 
.SECONDEXPANSION:
 
$(foreach OBJ,$(OBJECTS),$(eval $(OBJ)_DEPS = $(shell $(CC) $(CFLAGS) -MM $(OBJ:.o=.cpp) | sed s/.*://)))
%.o: %.cpp $$($$@_DEPS)
	$(CC) $(CFLAGS) $(LDLIBS) -c -o $@ $<
  
 
.PHONY : all install uninstall clean distclean gitadd
