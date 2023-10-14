# Makefile for C++ queue.
# Course: enel452
# Programmer: K. Naqvi

CCFLAGS = -Wall -Wextra -Wfloat-equal \
          -Wconversion -Wparentheses -pedantic -Wunused-parameter \
          -Wunused-variable -Wreturn-type -Wunused-function \
          -Wredundant-decls -Wreturn-type -Wunused-value \
          -Wswitch-default -Wuninitialized -Winit-self
CCFLAGS += -Werror
CCFLAGS += -g

testq : testq.o queue.o
	g++ $(CCFLAGS) -o $@ $^
queue.o : queue.cpp queue.h
testq.o : testq.cpp queue.h

%.o : %.cc
	g++ -c $(CCFLAGS) $< -o $@

%.o : %.cpp
	g++ -c $(CCFLAGS) $< -o $@

check: testq
	./$<

clean:
	rm -f *.o
	rm -f *~
	rm -f testq
	rm -f TAGS
