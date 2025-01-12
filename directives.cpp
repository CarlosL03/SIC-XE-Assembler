// Carlos Lopez, RED ID: 827117558
//David Granda-Ventura, RED ID: 824371438

#include "directives.h"
struct Directives {
    std::string name; // Change string to std::string
    int size;
};

const struct Directives directiveTable[] = {
        // Initialize the sizes of different directives
        {"START",0},
        {"END",0},
        {"RESB",1}, // Size of a byte in bytes
        {"RESW",3}, // Size of a word in bytes
        {"BYTE",1}, // Size of a byte in bytes (assuming one character)
        {"WORD",3}, // Size of a word in bytes
        {"BASE",0},
        {"LTORG",0},
        {"ORG",0},
        {"EQU",0},
        {"USE",0},
        {"CSECT",0},
        {"EXTDEF",0},
        {"EXTREF",0}
};

bool Directive::matchName(string check)
{
    bool flag = false;
    for(int i = 0; i < DIRECTIVE_TABLE_SIZE; i++)
    {
        if(directiveTable[i].name == check)
        {
            flag = true;
        }
    }
    return flag;
}

string Directive::getName(int index)
{
    return directiveTable[index].name;
}

int Directive::getSize(int mnemonic) {
    return directiveTable[mnemonic].size;
}
