// Carlos Lopez, RED ID: 827117558
//David Granda-Ventura, RED ID: 824371438
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> // for std::hex
#include <bitset>
#include <set>
#include <algorithm> // for std::find
#include <sstream> // for std::stringstream
#include <vector>
#include <string>
#include <cstdlib>

#include "opcode.h"
#include "directives.h"
#include "print.h"

const int MAX_ROWS = 1000;  // Maximum number of lines in the file
const int MAX_COLS = 3;     // Maximum number of columns per line

std::vector<std::string> commentLines;
int numComments = 0;
//All of these variables are declared here so that they are global, they can be accesed by any function in this file
std::string lines[MAX_ROWS][MAX_COLS];
//variables that serve as counters so we can initialize their respective arrays
int numRows = 0;
int numData = 0;
int numDirectives = 0;
int numInstructions = 0;
int numLabels = 0;
int numLits = 0;
int startAddress = 0;
int labelSize;
std :: string* LABELS;
//variables to handle base directive
int base = -1;
std::string baseNext = "";
int noBase = -1;
bool baseFlag = false;
bool noBaseFlag = false;
//creating variables of type OpCode and Directive in order to use their properties when
//assignning addresses
OpCode code = *new OpCode;
Directive directive = *new Directive;

void resetGlobalVars() {
    numComments = 0;
    numRows = 0;
    numData = 0;
    numDirectives = 0;
    numInstructions = 0;
    numLabels = 0;
    numLits = 0;
    startAddress = 0;
    labelSize = 0;
    base = -1;
    baseNext = "";
    noBase = -1;
    baseFlag = false;
    noBaseFlag = false;
}

// Function to read file and extract non-comment columns
void readFile(const std::string& filename, std::string lines[][MAX_COLS])
{
    std::ifstream file(filename.c_str());

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    numRows = 0;
    while (std::getline(file, line) && numRows < MAX_ROWS)
    {
        std::istringstream iss(line);
        std::string column;
        int colIndex = 0;

        if (!line.empty() && line[0] == '.')
        {
            // If it does, save the line to the vector
            commentLines.push_back(line);
            numComments++;
            continue;
        }


        while (iss >> column && colIndex < MAX_COLS)
        {
            //change so that it removes lines starting with '.'
            // Remove comments (lines starting with '.')
            //      if (column.find(".") != std::string::npos)
//            {
            //              column = column.substr(0, column.find("."));
            //        }

            // Remove leading and trailing whitespace from each column
            column.erase(0, column.find_first_not_of(" \t"));
            column.erase(column.find_last_not_of(" \t") + 1);


            // Store the non-comment column
            lines[numRows][colIndex] = column;
            colIndex++;

        }

        numRows++;
    }
    file.close();
}// end read in file

// Function to extract literals from a line
// Function to extract literals from a line
void extractLiterals(const std::string& line, std::vector<std::string>& literals)
{
    string litName;
    istringstream iss(line);
    string word;
    while (iss >> word) {
        if (word[0] == '=' && word[1] == '\'') {
            litName = word.substr(2, word.size()-2);
            literals.push_back(litName);
            numLits++;
        }
    }
}//end extractLiterals()

std::string* analyzeData(std::string lines[][MAX_COLS])
{
    //std::set<std::string> literals; // Set to store unique literals
    std::vector<std::string> literals;

    // Going through rows of the extracted program data array
    for (int i = 0; i < numRows; ++i)
    {
        //Goes through columns of the extracted program data array
        for (int j = 0; j < MAX_COLS; ++j)
        {
            if(j == 0 && !directive.matchName(lines[i][j]) && !code.matchName(lines[i][j]) && lines[i][j] != "")///we need to check for literals
            {
                numLabels++;
                numData++;
            }

            extractLiterals(lines[i][j], literals); // Extract literals from the line

            // As defined in Directives.cpp
            // This loop aims to find if theres an OpCode mnemonic in the rows of the program
            for (int k = 0; k < OP_TABLE_SIZE; ++k)
            {

                //checks if theres an assembler directive in the program
                if(k < DIRECTIVE_TABLE_SIZE && lines[i][j] == directive.getName(k))
                {
                    numDirectives ++;
                    //get start address of program, whether its 0 or a different number
                    if(lines[i][j] == "START")
                    {
                        startAddress = stoi(lines[i][j + 1]);
                    }

                }//end assembler directive Checks
                if (lines[i][j] == code.getName(k)) //check for mnumonic instructions
                {
                    numInstructions ++;
                }

            }//end comparison loop
        }//end columsloop
    }//end row loop;

    // storing found literals into an array
    std::string* numbers = new std::string[numLits];

    for (int zount = 0; zount < literals.size(); zount++) {
        string& literal = literals[zount];
        numbers[zount+1] = literal;
    }

    return numbers;//returning array of literals
}//end analyzeData

unsigned long long bitsetToUnsignedLongLong(const std::bitset<16>& bitset) {
    unsigned long long result = 0;
    for (size_t i = 0; i < bitset.size(); ++i) {
        result |= static_cast<unsigned long long>(bitset[i]) << i;
    }
    return result;
}
void baseValue(std::string labels[], int label_Loc[])
{
    //resets flag when function is called
    bool match = false;
    //checks if the BASE directive was set using symbol
    for(int i = 0; i < numLabels; i++)
    {
        if(baseNext == labels[i])
        {
            match = true;
            base = label_Loc[i];
        }
    }
    //else checks if BASE was set using a number
    if(!match)
    {
        try
        {
            // Attempt to convert the string to an integer
            base = stoi(baseNext);
        }
        catch (const std::exception&)
        {
            std::cout << "Not a valid BASE" << std::endl; // Conversion failed
        }
    }
}// end baseValue

bool isValidRegister(std::string registerTest)
{
    if(registerTest == "A" |registerTest == "X"|registerTest == "L"|registerTest == "B"|registerTest == "S"|registerTest == "T"|registerTest == "F")
    {
        return true;
    }
    else
    {
        return false;
    }
}
//This function helps assist checking the valid registers for format 2 instructions
std :: string getRegisterCode(std :: string registerTest)
{
    // Find the position of the comma
    size_t pos = registerTest.find(',');

    // If the comma is found, erase it
    if (pos != std::string::npos)
    {
        registerTest.erase(pos, 1); // Erase 1 character starting from 'pos'
    }

    if(registerTest == "A")
    {
        return "0000";
    }
    else if(registerTest == "X")
    {
        return "0001";
    }
    else if(registerTest == "L")
    {
        return "0010";
    }
    else if(registerTest == "B")
    {
        return "0011";
    }
    else if(registerTest == "S")
    {
        return "0100";
    }
    else if(registerTest == "T")
    {
        return "0101";
    }
    else if(registerTest == "F")
    {
        return "0110";
    }
    else
    {
        return "";//error
    }
}//end getRegisterCode

std :: string nix(std :: string str)
{
    //the following determines the first 3 bits of nixbpe
    if (str.find('#') != std::string::npos)
    {
        return "010";
    }
    else if (str.find('@') != std::string::npos)
    {
        return "100";
    }
    else if (str.find(",X") != std::string::npos)
    {
        return "111";
    }
    else
    {
        return "110";
    }

}//end nix
//checks if valid PC range
bool isPC(int offset)
{
    return (offset <= 2047 && offset >= -2048);
}//end isPC
//checks if valid BASE range
bool isBASE(int offset)
{
    return (offset < 4096 && offset >= 0);
}//end isBASE
// Checks if value is within 0 and 2^20 (1048576)
bool is_valid_extended(int value)
{
    return (value < 1048576 && value >= 0);
}//end is_valid_extended

unsigned long bitsetToUnsignedLong(const std::bitset<16>& bitset) {
    unsigned long result = 0;
    for (size_t i = 0; i < bitset.size(); ++i) {
        result |= static_cast<unsigned long>(bitset[i]) << i;
    }
    return result;
}

//easiest format, this function modifies the opcode from string to hex and returns it
std :: string format1(OpCode code, int index)
{
    std :: string concatenated = code.getOpCode(index);

    // Convert binary string to unsigned long long integer
    std::bitset<16> bits(concatenated);
    unsigned long long hexValue = bitsetToUnsignedLongLong(bits);

    // Convert integer to hexadecimal string
    std::stringstream ss;
    ss << std::hex << hexValue;
    std::string hexString = ss.str();

    return hexString;
}//end format1

std::string intToBinaryString(int number) {
    std::string binary;
    while (number > 0) {
        binary = (number % 2 == 0 ? "0" : "1") + binary;
        number /= 2;
    }
    return binary;
}

//REVIEW THE FIRST IF STATEMENT, WITH CLEAR TIXR SCV ETC.
//Handles format 2 instructions
std :: string format2(OpCode code, int index, std :: string registers)//registers is next line
{
    std :: string mCode = code.getOpCode(index);

    if(code.getName(index) == "CLEAR" || code.getName(index) == "TIXR"|| code.getName(index) == "SCV"|| code.getName(index) == "SHIRFTL"|| code.getName(index) == "SHIFTR")
    {
        std::string register1;
        std::string register2;
        std::string concatenated2;
        if(code.getName(index) == "CLEAR" || code.getName(index) == "TIXR")
        {
            if(isValidRegister(registers))
            {
                register2 = "0000";
                register1 = getRegisterCode(registers);

                concatenated2 = mCode +register1 +register2;

                // Convert binary string to unsigned long long integer
                std::bitset<16> bits(concatenated2);
                unsigned long long hexValue = bitsetToUnsignedLongLong(bits);;

                // Convert integer to hexadecimal string
                std::stringstream ss;
                ss << std::hex << hexValue;
                std::string hexString = ss.str();

                return hexString;
            }

        }//end CLEAR/TIXR check
        else if(code.getName(index) == "SHIFTL" || code.getName(index) == "SHIFTR")
        {
            if(isValidRegister(registers))
            {
                register1 = getRegisterCode(registers);
            }

            int regValue = stoi(registers);

            if(regValue >=0  && regValue <16)
            {
                register2 = intToBinaryString(regValue);//change number to hex
            }

            concatenated2 = mCode+register1+register2;

            // Convert binary string to unsigned long long integer
            std::bitset<16> bits(concatenated2);
            unsigned long long hexValue = bitsetToUnsignedLongLong(bits);

            // Convert integer to hexadecimal string
            std::stringstream ss;
            ss << std::hex << hexValue;
            std::string hexString = ss.str();
            return hexString;

        }//end SHIFT L/R check
        else if(code.getName(index) == "SVC")
        {
            int regValue = stoi(registers);

            //code to turn registers string to dec.
            if(regValue >=0  && regValue < 256)
            {
                register1 = intToBinaryString(regValue); //change to bin string
                //std :: cout << opcode.opcode << "+" << "Register 1 as hex/"<<std :: endl;
                concatenated2 = mCode + register1;

                // Convert binary string to unsigned long long integer
                std::bitset<16> bits(concatenated2);
                unsigned long long hexValue = bitsetToUnsignedLongLong(bits);

                // Convert integer to hexadecimal string
                std::stringstream ss;
                ss << std::hex << hexValue;
                std::string hexString = ss.str();

                return hexString;
            }
        }//end SVC check


        return "";
    }
    else //general format 2 instructions
    {
        std::string first, second;

        std::istringstream iss(registers);
        std::getline(iss, first, ','); // Read until the comma into 'first'
        std::getline(iss, second);      // Read the rest of the string into 'second'

        //std :: cout << "Result1:" << first << std :: endl;
        //std :: cout << "Result2:" << second << std :: endl;

        std :: string r1Code = getRegisterCode(first);
        std :: string r2Code = getRegisterCode(second);

        // Concatenate strings
        std::string concatenated = mCode + r1Code + r2Code;

        // Convert binary string to unsigned long long integer
        std::bitset<16> bits(concatenated);
        unsigned long long hexValue = bitsetToUnsignedLongLong(bits);

        // Convert integer to hexadecimal string
        std::stringstream ss;
        ss << std::hex << hexValue;
        std::string hexString = ss.str();

        //std::cout << "Hexadecimal representation: " << hexString << std::endl;

        //std :: cout << "Result is:" << concatenated << std :: endl;

        return hexString;
    }//end general format 2 machine code generation.
}// end format 2

//handles format 3 instructions
std :: string format3(OpCode code, int index, std :: string nextLine, int currentAddress, std::string labels[], int label_Loc[])
{
    if(code.getName(index) ==  "RSUB")
    {
        // Concatenate all strings
        std::string concatenated = code.getOpCode(index) + "000" + "000" + "000000000000";

        //std::cout << "Binary representation CONCAT: " << concatenated << std::endl;

        // Convert binary string to integer using bitset 32 for total number of bits in machine code
        int decimalValue = std::bitset<32>(concatenated).to_ulong();

        // Convert integer to hexadecimal string
        std::stringstream ss;
        ss << std::hex << std::setw(concatenated.length() / 4) << std::setfill('0') << decimalValue;
        std::string hexString = ss.str();

        //std::cout <<std::hex<< "Hexadecimal representation: " << hexString << std::endl;
        return hexString;
    }

    int offset = 0;
    int offset2 = 0;
    bool isLabel = false;//isSymbol??
    std :: string niX = nix(nextLine);

    //preparing NextLine by removing extra characters so we can compare them to our symbol table
    if (nextLine.find('#') != std::string::npos)
    {
        size_t pos = nextLine.find('#');
        nextLine.erase(pos, 1);
    }
    if (nextLine.find('@') != std::string::npos)
    {
        size_t pos = nextLine.find('@');
        nextLine.erase(pos, 1);
    }
    if (nextLine.find(",X") != std::string::npos)
    {
        size_t pos = nextLine.find(",X");
        nextLine.erase(pos, 2);
    }

    std :: string bpe = "000";//initializing, check if any inst, fall thru to "000"
    std :: string mCode = code.getOpCode(index);

    //checking if the NextLine matches a value in our symbol table
    for(int i = 0; i < numLabels ; i++)
    {
        if(nextLine == labels[i])
        {
            isLabel = true;
            offset = label_Loc[i];//to be used for PC relative calculations
            offset2 = label_Loc[i];//to be used for BASE relative calculations
        }
    }

    if(isLabel)//if the NextLine is a Symbol from symbol table
    {
        //perform offset calculation for PC Relative
        offset = offset - (currentAddress + 3);

        if(isPC(offset))//If offset is PC relative
        {
            //handles negative offsets/displacements
            if(offset < 0)
            {
                offset += 4096;
            }

            bpe = "010"; //setting flag variables for machine code, is not base, is PC, format 3 so not extended
        }//add an else if to check if ldb is used?
        else if(baseFlag)// if not PC then check for BASE relative
        {
            offset = offset2 - base;//BASE relative offset calculation

            if(isBASE(offset))//If offset is BASE relative
            {
                bpe = "100";//setting flag variables for machine code, is BASE, is not PC, format 3 so not extended
            }
        }
        else
        {
            std::cout<<"not valid pc or base"<< std::endl;
        }

    }//end check label if
    /*else//is a constant
    {

    }

     */

    ///////////non labels_address
    /*
     else
     {
         int value = 0;
         if (is_hex_string(operand)) {
             value = hex_to_int(strip_flag(operand));
         } else {
             value = dec_to_int(operand);
         }
         if (is_valid_base(value)) {
             line_iter->machinecode |= value;
         } else {
             cout << "ERROR: Constant Value " << operand << " out of bounds for format 3 on line " << line_iter->linenum
                  << endl;
             exit(73);
     }
     */

    //bit length for displacement/offset portion of format 3 machine code
    int width = 12;
    // Convert the offset/displacement integer to a binary string
    std::string binaryString = std::bitset<12>(offset).to_string();

    // If the binary string is shorter than the specified width, pad it with leading zeros
    if (binaryString.length() < width) {
        binaryString = std::string(width - binaryString.length(), '0') + binaryString;
    }

    // Concatenate all strings
    std::string concatenated = mCode + niX + bpe + binaryString;

    //std::cout << "Binary representation CONCAT: " << concatenated << std::endl;

    // Convert binary string to integer using bitset 32 for total number of bits in machine code
    int decimalValue = std::bitset<32>(concatenated).to_ulong();

    // Convert integer to hexadecimal string
    std::stringstream ss;
    ss << std::hex << std::setw(concatenated.length() / 4) << std::setfill('0') << decimalValue;
    std::string hexString = ss.str();

    //std::cout << "Hexadecimal representation: " << hexString << std::endl;

    return hexString;
}//end format3

//handles format 4
std :: string format4(OpCode code, int index, std :: string nextLine, std::string labels[], int label_Loc[], std::string* literals, int lit_Loc[])
{
    std::string hexString;
    std::stringstream hexStream;

    if(code.getName(index) ==  "RSUB")
    {
        // Concatenate all strings
        std::string binaryString = code.getOpCode(index) + "000" + "000" + "00000000000000000000";

        //std::cout << "Binary representation CONCAT: " << binaryString << std::endl;


        // Iterate over the binary string in groups of 4 bits
        for (size_t i = 0; i < binaryString.length(); i += 4)
        {
            // Extract the next 4 bits
            std::string group = binaryString.substr(i, 4);

            // Convert the 4-bit group to its hexadecimal equivalent
            int decimalValue = stoi(group, nullptr, 2); // Convert binary to decimal
            char hexValue = "0123456789ABCDEF"[decimalValue]; // Convert decimal to hexadecimal

            // Append the hexadecimal value to the stringstream
            hexStream << hexValue;
        }

        hexString = hexStream.str();
        //std::cout <<std::hex<< "Hexadecimal representation: " << hexString << std::endl;
        return hexString;
    }//end rsub check

    int address = 0;
    //std::string address_string;
    bool isLabel = false;//isSymbol??
    bool isLiteral = false;
    std :: string niX = nix(nextLine);

    //preparing NextLine by removing extra characters so we can compare them to our symbol table
    if (nextLine.find('#') != std::string::npos)
    {
        size_t pos = nextLine.find('#');
        nextLine.erase(pos, 1);
    }
    if (nextLine.find('@') != std::string::npos)
    {
        size_t pos = nextLine.find('@');
        nextLine.erase(pos, 1);
    }
    if (nextLine.find(",X") != std::string::npos)
    {
        size_t pos = nextLine.find(",X");
        nextLine.erase(pos, 2);
    }

    std :: string bpe = "001";//initializing, check if any inst, fall thru to "000"
    std :: string mCode = code.getOpCode(index);

    //checking if the NextLine matches a value in our symbol table
    for(int i = 0; i < numLabels ; i++)
    {
        if(nextLine == labels[i])
        {
            isLabel = true;
            address = label_Loc[i];
        }
    }

    std::string literal;

    //checking if the NextLine matches a value in our literal table
    if (nextLine.find("='")) {
            isLiteral = true;
    }

    if(isLiteral)
    {
        for(int i = 0; i < numLits; i++)
        {
            if (literal == literals[i]) {
                address = lit_Loc[i];
            }
        }
    }



    if(isLabel)//if the NextLine is a Symbol from symbol table
    {
        //bit length for displacement/offset portion of format 3 machine code
        int width = 12;
        // Convert the offset/displacement integer to a binary string
        std::string binaryString = std::bitset<20>(address).to_string();

        // If the binary string is shorter than the specified width, pad it with leading zeros
        if (binaryString.length() < width)
        {
            binaryString = std::string(width - binaryString.length(), '0') + binaryString;
        }

        // Concatenate all strings
        std::string concatenated = mCode + niX + bpe + binaryString;

        //std::cout << "Binary representation CONCAT: " << concatenated << std::endl;

        // Iterate over the binary string in groups of 4 bits
        for (size_t i = 0; i < concatenated.length(); i += 4)
        {
            // Extract the next 4 bits
            std::string group = concatenated.substr(i, 4);

            // Convert the 4-bit group to its hexadecimal equivalent
            int decimalValue = stoi(group, nullptr, 2); // Convert binary to decimal
            char hexValue = "0123456789ABCDEF"[decimalValue]; // Convert decimal to hexadecimal

            // Append the hexadecimal value to the stringstream
            hexStream << hexValue;
        }

    }//end check label if
    else if(isLiteral)//check if literal
    {
        //bit length for displacement/offset portion of format 3 machine code
        int width = 12;
        // Convert the offset/displacement integer to a binary string
        std::string binaryString = std::bitset<20>(address).to_string();

        // If the binary string is shorter than the specified width, pad it with leading zeros
        if (binaryString.length() < width)
        {
            binaryString = std::string(width - binaryString.length(), '0') + binaryString;
        }

        // Concatenate all strings
        std::string concatenated = mCode + niX + bpe + binaryString;

        //std::cout << "Binary representation CONCAT: " << concatenated << std::endl;

        // Iterate over the binary string in groups of 4 bits
        for (size_t i = 0; i < concatenated.length(); i += 4)
        {
            // Extract the next 4 bits
            std::string group = concatenated.substr(i, 4);

            // Convert the 4-bit group to its hexadecimal equivalent
            int decimalValue = stoi(group, nullptr, 2); // Convert binary to decimal
            char hexValue = "0123456789ABCDEF"[decimalValue]; // Convert decimal to hexadecimal

            // Append the hexadecimal value to the stringstream
            hexStream << hexValue;
        }
    }
    /*else//is a constant
    {

    }

     */

    ///////////non labels_address
    /*
     else
     {
         int value = 0;
         if (is_hex_string(operand)) {
             value = hex_to_int(strip_flag(operand));
         } else {
             value = dec_to_int(operand);
         }
         if (is_valid_base(value)) {
             line_iter->machinecode |= value;
         } else {
             cout << "ERROR: Constant Value " << operand << " out of bounds for format 3 on line " << line_iter->linenum
                  << endl;
             exit(73);
     }
     */

    hexString = hexStream.str();
    //std::cout << "Hexadecimal representation: " << hexString << std::endl;

    return hexString;
}//end format4

//the goal of the first pass function is to:
//1) Assign addresses to all statements in the program
//2) Save the values assigned to each label for use in Pass2
//3) Perform some processing of assembler directives
void firstPass(std::string fileName, std::string lines[][MAX_COLS], std::string instructions[], std::string nextInstructions[], std::string labels[], int label_Loc[], int inst_Loc[], std::string* literals, int lit_Loc[], int location[], std::string lit_Code[])
{
    int dir_Count = 0;
    int lab_Count = 0;
    int ins_Count = 0;
    int lit_Count = 0;
    int inst_loc_Count = 0;
    int next_Count = 0;
    int lab_address_Count = {0};
    int index_labes[numRows];
    location[0] = startAddress;
    int previousAddress = startAddress;

    // Going through rows of the extracted program data array
    for (int i = 0; i < numRows; ++i)
    {
        //Goes through columns of the extracted program data array
        // and Updates address based on mnemonic size
        for (int j = 0; j < MAX_COLS; ++j)
        {
            if(j == 0 && !directive.matchName(lines[i][j]) && !code.matchName(lines[i][j]) && lines[i][j] != "")///we need to check for literals
            {
                labels[lab_Count++] = lines[i][j];
                label_Loc[lab_address_Count++]= startAddress;
            }



            // This loop aims to find if theres an OpCode mnemonic in the rows of the program
            //Based on our OpCode.cpp the total ammount of opcodes to search for is 101
            for (int k = 0; k < OP_TABLE_SIZE; ++k)
            {
                //checks if theres an assembler directive in the program
                if(k < DIRECTIVE_TABLE_SIZE && lines[i][j] == directive.getName(k))//added an = to directive_index > 0;
                {
                    //Checks if the assembler directive is RESW because RESW size depends on the value next to it
                    // multiplied by 3(the size of a WORD)
                    //if not, then just add the size to our address counter like normal.
                    if(directive.getName(k) == "RESW")
                    {
                        startAddress += directive.getSize(k) * stoi(lines[i][j + 1]);
                    }
                    else if(lines[i][j] == "BASE")
                    {
                        baseNext = lines[i][j + 1];
                        baseFlag = true;
                    }
                    else
                    {
                        startAddress += directive.getSize(k);
                    }
                }//end assembler directive Checks

                //check for mnumonic instructions
                if (lines[i][j] == code.getName(k))
                {
                    inst_Loc[inst_loc_Count++] = startAddress;
                    //add instruction size to address counter
                    startAddress += code.getSize(k);//increase address size
                    instructions[ins_Count++] = code.getName(k);//store instruction in an array
                    if(code.getSize(k) != 1)
                    {
                        nextInstructions[next_Count++] = lines[i][j + 1];
                    }
                }
            }//end comparison loop
        }//end columsloop

        previousAddress = startAddress;
        //for each row in the program, we save an associated address
        location[i+1] = startAddress;

    }//end row loop

    //print out to symbol table file
    printToSymbolTable(fileName+".st",labels,label_Loc,numLabels, literals, lit_Loc, numLits, lit_Code);

}//end first pass function

void secondPass(std::string fileName, std::string lines[][MAX_COLS], std::string instructions[], std::string nextInstructions[], std::string labels[], int label_Loc[], int inst_Loc[], std::string machine_array[], int location[], std::string* literals, int lit_Loc[], std::string lit_Code[])
{
    std::string machineCode;
    bool baseFlag = false;

    for(int i = 0; i < numInstructions; ++i)
    {
        for(int k = 0; k < OP_TABLE_SIZE; k++)
        {
            if(instructions[i] == code.getName(k))
            {
                //next check instructions
                if(code.getSize(k) == 4)
                {
                    //handle format 4
                    machine_array[i] = format4(code, k, nextInstructions[i], labels, label_Loc, literals, lit_Loc);
                }
                else if(code.getSize(k) == 3)
                {
                    //handle format 3
                    machine_array[i] = format3(code, k, nextInstructions[i], inst_Loc[i], labels, label_Loc);
                }
                else if(code.getSize(k) == 2)
                {
                    //handle format 2
                    machine_array[i] = format2(code, k, nextInstructions[i]);
                }
                else
                {
                    //handle format 1
                    machine_array[i] = format1(code, k);
                }
            }
        }//end inner for loop
    }//end outer for loop

    //print out to library file
    printToLibrary(fileName + ".l", lines,location, numRows, machine_array, lit_Code, numComments, commentLines);
}//end second pass function



int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Insufficient input given: no files to open. Terminating program.\n";
        return 1;
    }
    cout << "Input detected. Assembling source file(s).\n";
    int c=1;
    while(c < argc) {
        int numLines = 0;
        int i = 0;
        ifstream inFile;
        string filename = argv[c];
        size_t lastDotIndex = filename.find_last_of(".");
        string fName = filename.substr(0, lastDotIndex);
        inFile.open(filename);
        cout << "Now assembling: " << fName << endl;

        //call to readFile, read in Sic/XE file information
        readFile(filename, lines);

        string *literals = analyzeData(lines);

        string instructions[numInstructions];
        string nextInstructions[numInstructions];
        string labels[numLabels];
        int label_Loc[numLabels];
        int inst_Loc[numInstructions];
        string machine_array[numInstructions];
        int lit_Loc[numLits];
        int location[numRows];
        string lit_Code[numLits];

        //Begin performing the first pass of a two pass assembler
        firstPass(fName, lines, instructions, nextInstructions, labels, label_Loc, inst_Loc, literals, lit_Loc, location,
                  lit_Code);

        //if There is a BASE Directive, we will obtain the base stored value, and apply it to our base global variable
        if (baseFlag) {
            baseValue(labels, label_Loc);
        }
        //Move on to the second pass of a two pass assembler
        secondPass(fName, lines, instructions, nextInstructions, labels, label_Loc, inst_Loc, machine_array, location,
                   literals, lit_Loc, lit_Code);

        c++;
        resetGlobalVars();
    }
    return 0;
}