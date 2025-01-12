#######################################################
# Makefile for CS530 Assignment #2
# Carlos Lopez, RED ID: 827117558
# David Granda-Ventura, RED ID: 824371438
#######################################################

EXEC = asl
FILES = asxe.cpp opcode.cpp directives.cpp print.cpp
CXX = g++
CXXFLAGS = -std=c++11

asl:
	$(CXX) $(CXXFLAGS) $(FILES) -o $@
	./asl sampleXE.txt

###################[ EOF: Makefile ] ###################