// Carlos Lopez, RED ID: 827117558
//David Granda-Ventura, RED ID: 824371438

#include "print.h"

#include <iostream>
#include <fstream>
#include <iomanip> // for std::hex
#include <vector>
#include "opcode.h"
using namespace std;

OpCode codes = *new OpCode;

// Function to print the data to a Symbol Table
void printToSymbolTable(const std::string& filename, std::string lables[], int labels_address[], int counter, std::string* literals, int lit_Loc[], int numLits, std::string lit_Code[])
{
    std::ofstream outfile(filename);

    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    // Print CSect Symbol Value LENGTH Flags
    outfile << std::left << std::hex << std::setw(8) << "CSect" << std::setw(8) << "Symbol" << std::setw(8) << "Value"
            << std::setw(8) << "LENGTH" << std::setw(8) << "Flags:" << std::endl;
    cout << std::left << std::hex << std::setw(8) << "CSect" << std::setw(8) << "Symbol" << std::setw(8) << "Value"
            << std::setw(8) << "LENGTH" << std::setw(8) << "Flags:" << std::endl;
    outfile << "--------------------------------------" << std::endl;
    cout << "--------------------------------------" << std::endl;
    //finished formatting!! took 3 hours
    outfile << std::left << std::uppercase << std::setw(6) << lables[0] << "  " << std::setw(6) << "        "/*symbol.name*/ << std::setw(6) << setfill('0') << std::right << labels_address[0]/*symbol.value*/ << "  "
            << std::setw(6) << std::hex << (labels_address[counter - 1] +3 )<< setfill(' ') << std::left<< std::setw(6)  << ""<< std::endl;
    cout << std::left << std::uppercase << std::setw(6) << lables[0] << "  " << std::setw(6) << "        "/*symbol.name*/ << std::setw(6) << setfill('0') << std::right << labels_address[0]/*symbol.value*/ << "  "
            << std::setw(6) << std::hex << (labels_address[counter - 1] +3 )<< setfill(' ') << std::left<< std::setw(6)  << ""<< std::endl;
    //need to get num rows in here to get correct size
    int z = 0;
    for (int i = 1 ; i < counter ; i++)
    {
        outfile << std::setw(8) << "" << std::setw(6) << lables[i]/*symbol.name*/<< "  " << std::right << std::setw(6) << setfill('0') << labels_address[z+1]/*symbol.value*/ << "  "
                << std::setw(6) << std::left << setfill(' ') << ""/*symbol.length*/ << "  " << std::setw(6) << "R"/*symbol.flags*/ << std::endl;

        cout << std::setw(8) << "" << std::setw(6) << lables[i]/*symbol.name*/<< "  " << std::right << std::setw(6) << setfill('0') << labels_address[z+1]/*symbol.value*/ << "  "
             << std::setw(6) << std::left << setfill(' ') << ""/*symbol.length*/ << "  " << std::setw(6) << "R"/*symbol.flags*/ << std::endl;
        z++;

    }

    outfile << " " << std::endl;
    cout << " " << endl;

    outfile << std::setw(6) << "Literal Table" << std::endl << std::setw(6) << "Name" << std::setw(8) << "Operand"
            << std::setw(8) << "Address" << std::setw(8) << "Length:" << std::endl;
    outfile << "--------------------------------------" << std::endl;

    cout << std::setw(6) << "Literal Table" << std::endl << std::setw(6) << "Name" << std::setw(8) << "Operand"
                           << std::setw(8) << "Address" << std::setw(8) << "Length:" << std::endl;
    cout << "--------------------------------------" << std::endl;

    //int z = 0;
    for (int i = 0 ; i < numLits ; i++)
    {
        cout << std::setw(6) << literals[i]/*symbol.name*/ << std::setw(8) << lit_Code[i] /*symbol.value*/
                << std::setw(8) << lit_Loc[i]/*symbol.length*/ << std::setw(8) << literals[i].length()/*symbol.flags*/ << std::endl;
        outfile << std::setw(6) << literals[i]/*symbol.name*/ << std::setw(8) << lit_Code[i] /*symbol.value*/
                << std::setw(8) << lit_Loc[i]/*symbol.length*/ << std::setw(8) << literals[i].length()/*symbol.flags*/ << std::endl;
    }

    outfile.close();
    std::cout << "Data has been written to " << filename << std::endl;
}//end printing to Symbol Table

// Function to print the data to a Library
void printToLibrary(const std::string& filename, std::string lines[][MAX_COLMS], int location[], int numRows, std::string machine_array[], std::string lit_Code[], int numColumns, std::vector<std::string>& commentLines) {
    std::ofstream outfile(filename);

    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    int count = 0;
    int countLit = 0;
    bool instruct = false;
    bool isLiteral = false;

    // Print the comment lines
    for (int i = 0; i < commentLines.size(); i++) {
        auto& line = commentLines[i];
        outfile << line << endl;
    }

    // Print the extracted data
    //still need some formatting fixes, specifically if a row doesnt have an entry, we need to leave a blank space.
    //not let the row shrink in size.
    // also once the machine code logic is implemented, we need to print each code at the end of each
    //line that requires it.

    for (int i = 0; i < numRows; ++i) {
        int numBlanks = 0;

        outfile << /*"Address: " << */std::right << std::setw(4) << std::setfill('0') << std::uppercase << std::hex << location[i]/*address*/<< std::left << std::setfill(' ') << "  ";// << std::endl;
        cout << /*"Address: " << */std::right << std::setw(4) << std::setfill('0') << std::uppercase << std::hex << location[i]/*address*/<< std::left << std::setfill(' ') << "  ";
        for (int b = 0; b < MAX_COLMS; b++) {
            if (lines[i][b] == "" || lines[i][b] == " " || lines[i][b] == "  " || lines[i][b] == "   "){
                numBlanks++;
                //cout << i <<","<< j<< endl;
            }
        }
        if (lines[i][0] == "*"){
            numBlanks = 0;
        }
        for (int j = 0; j < MAX_COLMS; ++j)
        {
            //if(lines[i][j] == code.getName(k)){}
            if (numBlanks == 0) {
                outfile << std::left << std::setw(6) << std::setfill(' ')<<lines[i][j] << std :: left << std::setw(4) << std::setfill(' ')<<" ";
                cout << std::left << std::setw(6) << std::setfill(' ')<<lines[i][j] << std :: left << std::setw(4) << std::setfill(' ')<<" ";
            }
            if (numBlanks == 1) {
                if (j == 0) {
                    outfile << std::setw(6) << std::setfill(' ') << " " << std::setw(4) << std::setfill(' ')<<" ";
                    cout << std::setw(6) << std::setfill(' ') << " " << std::setw(4) << std::setfill(' ')<<" ";
                    string temp = lines[i][j];
                    lines[i][2] = lines[i][1];
                    lines[i][1] = temp;
                }
                else {
                    outfile << std::left << std::setw(6) << std::setfill(' ')<<lines[i][j] << std :: left << std::setw(4) << std::setfill(' ')<<" ";
                    cout << std::left << std::setw(6) << std::setfill(' ')<<lines[i][j] << std :: left << std::setw(4) << std::setfill(' ')<<" ";
                }
            }
            if (numBlanks == 2) {
                if (j == 1) {
                    outfile << std::left << std::setw(6) << std::setfill(' ')<<lines[i][0] << std :: left << std::setw(4) << std::setfill(' ')<<" ";
                    cout << std::left << std::setw(6) << std::setfill(' ')<<lines[i][0] << std :: left << std::setw(4) << std::setfill(' ')<<" ";
                }
                else {
                    outfile << std::setw(6) << std::setfill(' ') << " " << std::setw(4) << std::setfill(' ')<<" ";
                    cout << std::setw(6) << std::setfill(' ') << " " << std::setw(4) << std::setfill(' ')<<" ";
                }
            }
            //outfile << j;
            //outfile << std::setw(5) << std::setfill(' ')<<lines[i][j] << std :: left << std::setw(5) << std::setfill(' ')<<"\t";

            for (int k = 0; k < OP_TABLE_SIZE; ++k)
            {
                if(lines[i][j] == codes.getName(k))
                {
                    instruct = true;
                    //outfile << machine_array[count++]<<"\t";
                }
                /*else if(lines[i][j].find("=\'"))//check if literal but no instruction
                {
                    isLiteral = true;
                    std::string literal = lines[i][j];
                }*/

            }
        }

        if(instruct)
        {
            string outp = machine_array[count++];
            outfile << outp <<"\t"<<std::endl;
            cout << outp <<"\t"<<std::endl;
        }
        /*else if(isLiteral)
        {
            outfile << lit_Code[countLit++] << "\t"<<std::endl;
        }*/
        else
        {
            cout << endl;
            outfile << std::endl;
        }
        //outfile <<std::endl;
        instruct = false;
        isLiteral = false;
    }


    outfile.close();
    std::cout << "Data has been written to " << filename << std::endl;
}//end printing to library