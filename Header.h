//
//  Header.h
//  CompDesign
//
//  Created by Ariel Taveras on 12/10/16.
//  Copyright © 2016 Ariel Taveras. All rights reserved.
//

#ifndef Header_h
#define Header_h


#endif /* Header_h */

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
using namespace std;

// ALL OF THE ABOVE SHOULD GO IN A HEADER FILE !!!!!!!!
// Enumerate Instruction Types for use throughout the program.
enum InstrType { LD, ADD, SUB, MULT, DIV, SD, ADDI, SUBI, BEQ, BNE};
InstrType GetInstructionEnum(string instr);

void PromtUserForHardwareInfo();
void IssueInstruction(InstrType instr, string dest, string R1, string R2);
void IssueInstruction(InstrType instr, string dest, string R1, string R2);
void ExecuteIssuedInstructions();
bool CheckForInputDependence(string R1, string R2);
bool LoadInstructionIntoReservationStation(InstrType type, string instr, string dest, string R1, string R2);
void Print_Registers_Memory();
class PrintTable {
public:
    deque<string> Instructions;
    deque<string> list;
}PrintOut;


