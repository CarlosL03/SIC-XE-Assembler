// Carlos Lopez, RED ID: 827117558
//David Granda-Ventura, RED ID: 824371438
#ifndef OPCODE_H
#define OPCODE_H
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>

// Define the opCodeTable array outside of any class or function
#define OP_TABLE_SIZE 101

using namespace std;

class OpCode {
public:
    bool matchName(string check);
    string getName(int index);
    int getSize(int index);
    string getOpCode(int index);
};

#endif
