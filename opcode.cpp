// Carlos Lopez, RED ID: 827117558
//David Granda-Ventura, RED ID: 824371438

#include "opcode.h"

struct opCode {
    std::string name; // Change string to std::string
    std :: string opCode; //quickly changing from int to string
    int format; // format 3 refers to format 3 or format 4
};

const struct opCode opCodeTable[] = {//for any entry in format 3 or 4 we write the opcode as binary with the last two bits masked out.
//for formats 1 and 2 we write the full opcode in 8bits to keep consistent format.
        {"ADD", "000110", 3},  {"ADDF", "010110", 3},   {"ADDR", "10010000", 2},   {"AND", "010000", 3},  {"CLEAR", "10110100", 2},
        {"COMP", "001010", 3}, {"COMPF", "100010", 3},  {"COMPR", "10100000", 2},  {"DIV", "001001", 3},  {"DIVF", "011001", 3},
        {"DIVR", "10011100", 2}, {"FIX", "11000100", 1},    {"FLOAT", "11000000", 1},  {"HIO", "11110100", 1},  {"J", "001111", 3},
        {"JEQ", "001100", 3},  {"JGT", "001101", 3},    {"JLT", "001110", 3},    {"JSUB", "010010", 3}, {"LDA", "000000", 3},
        {"LDB", "011010", 3},  {"LDCH", "010100", 3},   {"LDF", "011100", 3},    {"LDL", "000010", 3},  {"LDS", "011011", 3},
        {"LDT", "011101", 3},  {"LDX", "000001", 3},    {"LPS", "110100", 3},    {"MUL", "001000", 3},  {"MULF", "011000", 3},
        {"MULR", "10011000", 2}, {"NORM", "11001000", 1},   {"OR", "010001", 3},     {"RD", "110110", 3},   {"RMO", "10101100", 2},
        {"RSUB", "010011", 3}, {"SHIFTL", "10100100", 2}, {"SHIFTR", "10101000", 2}, {"SIO", "11110000", 1},  {"SSK", "111011", 3},
        {"STA", "000011", 3},  {"STB", "011110", 3},    {"STCH", "010110", 3},   {"STF", "100000", 3},  {"STI", "110101", 3},
        {"STL", "000101", 3},  {"STS", "011111", 3},    {"STSW", "111010", 3},   {"STT", "100001", 3},  {"STX", "000100", 3},
        {"SUB", "000111", 3},  {"SUBF", "010111", 3},   {"SUBR", "10010100", 2},   {"SVC", "10110000", 2},  {"TD", "111000", 3},
        {"TIO", "11111000", 1},  {"TIX", "001011", 3},    {"TIXR", "10111000", 2},   {"WD", "110111", 3},
        {"+ADD", "000110", 4},  {"+ADDF", "010110", 4},    {"+AND", "010000", 4},   {"+COMP", "001010", 4},
        {"+COMPF", "100010", 4},  {"+DIV", "001001", 4},    {"+DIVF", "011001", 4},   {"+J", "001111", 4},
        {"+JEQ", "001100", 4},  {"+JGT", "001101", 4},    {"+JLT", "001110", 4},   {"+JSUB", "010010", 4},
        {"+LDA", "000000", 4},  {"+LDB", "011010", 4},    {"+LDCH", "010100", 4},   {"+LDF", "011100", 4},
        {"+LDL", "000010", 4},  {"+LDS", "011011", 4},    {"+LDT", "011101", 4},   {"+LDX", "000001", 4},
        {"+LPS", "110100", 4},  {"+MUL", "001000", 4},    {"+MULF", "011000", 4},   {"+OR", "010001", 4},
        {"+RD", "110110", 4},  {"+RSUB", "010011", 4},    {"+SSK", "111011", 4},   {"+STA", "000011", 4},
        {"+STB", "011110", 4},  {"+STCH", "010110", 4},    {"+STF", "100000", 4},   {"+STI", "110101", 4},
        {"+STL", "000101", 4},  {"+STS", "011111", 4},    {"+STSW", "111010", 4},   {"+STT", "100001", 4},
        {"+STX", "000100", 4},  {"+SUB", "000111", 4},    {"+SUBF", "010111", 4},   {"+TD", "111000", 4},
        {"+TIX", "001011", 4},  {"+WD", "110111", 4}
};

string OpCode::getOpCode(int index)
{
    return opCodeTable[index].opCode;
}

string OpCode::getName(int index)
{
    return opCodeTable[index].name;
}

bool OpCode::matchName(string check)
{
    bool flag = false;
    for(int i = 0; i < OP_TABLE_SIZE; i++)
    {
        if(opCodeTable[i].name == check)
        {
            flag = true;
        }
    }
    return flag;
}

int OpCode::getSize(int mnemonic) {
    return opCodeTable[mnemonic].format;
}