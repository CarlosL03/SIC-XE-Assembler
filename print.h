//Carlos Lopez, RED ID: 827117558
//David Granda-Ventura, RED ID: 824371438
#ifndef PRINT_H
#define PRINT_H

#include <string>
#include <vector>


const int MAX_COLMS = 3;

void printToSymbolTable(const std::string& filename, std::string labels[], int labels_address[], int counter, std::string* literals, int lit_Loc[], int numLits, std::string lit_Code[]);
void printToLibrary(const std::string& filename, std::string lines[][MAX_COLMS], int location[], int numRows, std::string machine_array[], std::string lit_Code[], int numColums, std::vector<std::string>& commentLines);

#endif
