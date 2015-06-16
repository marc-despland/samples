SHELL = /bin/sh
CC    = gcc
 
FLAGS        = -Iinclude
CFLAGS       = -Wall
 
TARGET  = sampletty
SOURCES = $(shell echo src/*.c)
HEADERS = $(shell echo include/*.h)
OBJECTS = $(SOURCES:.c=.o)
DEBUGFLAGS = -g
 
all: $(TARGET)
 
$(TARGET): $(OBJECTS) $(HEADERS)
  $(CC) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) -o $(TARGET) $(OBJECTS)
 
clean:
  -rm -f $(OBJECTS)
  -rm -f gmon.out
 
distclean: clean
  -rm -f $(TARGET)
 
 
.SECONDEXPANSION:
 
$(foreach OBJ,$(OBJECTS),$(eval $(OBJ)_DEPS = $(shell gcc -MM $(OBJ:.o=.c) | sed s/.*://)))
%.o: %.c $$($$@_DEPS)
  $(CC) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) -c -o $@ $<
  
 
.PHONY : all install uninstall clean distclean
