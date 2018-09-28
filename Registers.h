//
//  Registers.h
//  CompDesign
//
//  Created by Ariel Taveras on 12/10/16.
//  Copyright © 2016 Ariel Taveras. All rights reserved.
//

#ifndef Registers_h
#define Registers_h


#endif /* Registers_h */

//#include "TomasuloTable.h"
////#include "Registers.h"
//#include "Memory.h"
//#include "ROB.h"
//#include "Loader.h"
//#include "Adder_Sub.h"
//#include "Multiplier.h"
//#include "Divider.h"
//#include "Storer.h"

class Registers {
public:
    Registers();
    int GetDataFromReg(string reg);
    void StoreValueInReg(string reg, int value);
    void LoadFromMemory(string reg, string mem);
    bool TakeRegToken(string dest);
    bool CheckIfAvailable(string R1, string R2);
    void ReleaseRegToken(string dest);
    bool TakeROB_RegToken(string dest);
    void ReleaseROB_RegToken(string commited_dest_reg);
    vector<float> REG;
    int Int_REG[32];
    vector<bool> Busy_flag;
    vector<bool> In_Use;
    vector<bool> ROB_token;
}Registers;

Registers::Registers() {
    REG.resize(32,0);
    Busy_flag.resize(32,0);
    ROB_token.resize(32,0);
    In_Use.resize(32,0);
    for (int i=0; i < 32; i++){
        REG[i] = 0;
        Int_REG[i] = i;
        Busy_flag[i] = false;
        In_Use[i] = false;
        ROB_token[i] = false;
    }
    return;
}

int Registers::GetDataFromReg(string reg) {
    // strip the 'F' or '$' from register/memory name and convert the remaining string into an integer number
    string reg_index = reg.erase(0,1);
    int reg_data = REG[ stoi(reg_index) ];
    return reg_data;
}
void Registers::StoreValueInReg(string reg, int value) {
    string reg_index = reg.erase(0,1);
    REG[ stoi(reg_index) ] = value;
}

bool Registers::CheckIfAvailable(string R1, string R2) {
    
    string reg_index = R1.erase(0,1);
    int index = stoi(reg_index);
    if (R2[0] == '$') {
        if(In_Use[index]) {
            return true;
        }
        else {
            return false;
        }
    }
    if(R2 == "no input") {
        if(In_Use[index]) {
            return true;
        }
        else {
            return false;
        }
    }
    string reg_index2 = R2.erase(0,1);
    int index2 = stoi(reg_index2);
    
    if (In_Use[index]) {
        return true;
    }
    if (In_Use[index2]) {
        return true;
    }
    return false;
}

bool Registers::TakeRegToken(string dest) {
    string reg_index = dest.erase(0,1);
    int index = stoi(reg_index);
    if(In_Use[index]) {
        return false;
    }
    else {
        In_Use[index] = true;
    }
    
    //cout << "Registers.TakeRegToken did not work properly" << endl;
    return false;
}

void Registers::ReleaseRegToken(string dest) {
    string reg_index = dest.erase(0,1);
    int index = stoi(reg_index);
    In_Use[index] = false;
    return;
}

// check if that registers ROB token is already assigned ^^^^^^^
bool Registers::TakeROB_RegToken(string dest) {
    string dest_reg_location = dest.erase(0,1);
    int dest_index = stoi(dest_reg_location);
    if (ROB_token[dest_index]) {
        return false; // false inidcates that this instructions output register cannot be commited to b/c it still needs to be used by a previously issued instruction that has not fininished executing yet.
    }
    ROB_token[dest_index] = true; // indicate that a the current has taken this token.
    return true; // register is free and this instruction can now commit to it.
}

void Registers::ReleaseROB_RegToken(string commited_dest_reg) {
    string dest_reg_location = commited_dest_reg.erase(0,1);
    int dest_index = stoi(dest_reg_location);
    ROB_token[dest_index] = false;
}
