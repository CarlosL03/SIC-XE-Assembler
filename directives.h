// Carlos Lopez, RED ID: 827117558
//David Granda-Ventura, RED ID: 824371438
#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>

#define DIRECTIVE_TABLE_SIZE 14

using namespace std;

class Directive {
public:
    bool matchName(string check);
    string getName(int index);
    int getSize(int index);
};

#endif
