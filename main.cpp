
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "directives.h"
#include "opcode.h"
#include "print.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Insufficient input given: no files to open. Terminating program.\n";
        return 1;
    }
    cout << "Input detected. Assembling source file(s).\n";
    int c=1;
    while(c < argc) {
        vector<vector<string>> code(1000);
        vector<int> location;
        map<string,string> symbolTable;
        map<string,string>opTable;

        vector<int>objectCodeInt;
        vector<string>objectCode;
        vector<vector<string>>literalTable(500);

        int lines = readFile(code);
        int baseAddress;

        addressCounter(code,location,literalTable,lines-1);

        createSymbolTable(code,location,symbolTable,lines-1);
        printSymbolTable(symbolTable);
        printLiteralTable(literalTable);


    }

}