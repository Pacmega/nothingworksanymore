#
#	Makefile for Sluice
#
#	© Joran van Teeffelen & Bas Witters
#

TARGET = sluice

FILES = code/*.cpp
HEADERS = code/*.h
LIB = code/lib/*.c

LIBS = -lm
LDLIBS = -lrt
CFLAGS = -Wall -Werror -o

CC = g++

.PHONY: default all clean

cm: clean sluice
	
sluice: $(FILES) Makefile $(HEADERS) 
	@$(CC) $(FILES) $(LIB) $(CFLAGS) $(TARGET)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
