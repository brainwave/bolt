CC=gcc
CXX=g++

PROG=main.cpp
PROG2=display_png.cpp
CXXFLAGS=--std=c++11 `libpng-config --cflags`
INCLUDE_DIRS =-I/home/akshay/workspace/bolt_akshay/bolt_code/libs
LDFLAGS=-g 
LDLIBS =-lglfw -lGL -lGLEW `libpng-config --ldflags`
OP1=-oslicer.o
OP2=-oviewer.o

COMPILE_COMMAND=display_png.cpp -o viewer.o -lSDL -lGL

main:
		$(CXX)	$(PROG)  $(LDFLAGS) $(OP1) $(INCLUDE_DIRS)  $(LDLIBS) $(CXXFLAGS)

viewer:
		$(CXX) $(COMPILE_COMMAND) $(INCLUDE_DIRS)
