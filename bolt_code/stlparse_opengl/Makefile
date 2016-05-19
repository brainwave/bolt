#Compiler env vars
CC=gcc
CXX=g++

#Prog env vars
PROG=main.cpp
PROG2=display_dat.cpp
CXXFLAGS=--std=c++11
INCLUDE_DIRS =-I/home/brainwave/glStl/libs/ 
LDFLAGS=-g 
LDLIBS =-lglfw -lGL -lGLEW 
OP1=-oslicer.o
OP2=-oviewer.o

#valid targets
main:
		$(CXX)	$(PROG)  $(LDFLAGS) $(OP1) $(INCLUDE_DIRS)  $(LDLIBS) $(CXXFLAGS)

display_dat:
		$(CXX) $(PROG2) $(LDFLAGS) $(OP2) $(INCLUDE_DIRS) $(LDLIBS) $(CXXFLAGS)
