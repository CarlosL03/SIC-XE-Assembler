# READ ME
This repository details the second programming project for the CS-530: Systems Programming course at SDSU. 
This was a group project completed by Carlos Lopez and David Granda-Ventura over two months in the Spring of 2024 semester.

## Project Description
The purpose of this team project is to demonstrate essential software engineering and course learning concepts with C/C++ programming skills by creating a two-pass SIC/XE assembler. This assembler will be able to open given XE source files and generate a listing file and SYMTAB file from the instructions. This assembler will use the OPTAB and SYMTAB data structures to look up mnemonic operations codes to translate to machine language and store addresses to labels, respectively. These addresses are found by using the LOCCTR variable that is initialized to the address specified in the START statement of the input source file.

## Compile Instructions
This project was compiled using edoras and the Makefile. I wrote the makefile in different ways and one issue is that it doesn’t always make and it believes that the executable is already up to date.

## Operating Instructions
To compile without the Makefile you can enter ‘g++ -std=c++11 asxe.cpp -o asxe opcode.cpp directives.cpp print.cpp’ followed by ./asxe sampleXE.txt OR ./asxe START.txt to execute the SIC/XE source file(s).
This program can also be executed without any input arguments at all with simply just ./asxe entered and will output a message stating that there was insufficient input given and that the file will terminate. However, it was typically always tested with one input file given. It loops through each argument found in argv following ./asxe.

##
