//
//  Memory.h
//  CompDesign
//
//  Created by Ariel Taveras on 12/10/16.
//  Copyright © 2016 Ariel Taveras. All rights reserved.
//

#ifndef Memory_h
#define Memory_h

#endif /* Memory_h */

class Memory {
public:
    //Constructor.
    Memory();
    int GetDataFromMem(string mem, string offset);
    int GetDataFromMem2(string mem);
    void StoreValueInMem(int address, int value);
    int GetAddress(string location, string offset);
    int MEM[19];
private:
    bool flag;
}Memory;

Memory::Memory() {
    
    for(int i=0; i < 19; i++){
        MEM[i] = 0;
    }
    MEM[0] = 45;
    MEM[1] = 12;
    MEM[2] = 0;
    MEM[3] = 0;
    MEM[4] = 10;
    MEM[5] = 135;
    MEM[6] = 254;
    MEM[7] = 127;
    MEM[8] = 18;
    MEM[9] = 4;
    MEM[10] = 55;
    MEM[11] = 8;
    MEM[12] = 2;
    MEM[13] = 98;
    MEM[14] = 13;
    MEM[15] = 5;
    MEM[16] = 233;
    MEM[17] = 158;
    MEM[18] = 167;
    //MEM[19] = 1;
    return;
}

int Memory::GetDataFromMem2(string mem) {
    string mem_index = mem.erase(0,1);
    int address = stoi(mem_index);
    return MEM[address];
}

int Memory::GetDataFromMem(string mem, string offset) {
    string mem_index = offset.erase(0,1);
    // GET VALUE STORED IN THIS INTEGER REGISTER AND ADD TO mem number for address.
    int location_offset = Registers.Int_REG[stoi(mem_index)];
    int address = location_offset + stoi(mem);
    int mem_data = MEM[address];
    return mem_data;
}
int Memory::GetAddress(string location, string offset) {
    string mem_index = offset.erase(0,1);
    int location_offset = Registers.Int_REG[stoi(mem_index)];
    int address = location_offset + stoi(location);
    return address;
}

void Memory::StoreValueInMem(int address, int value) {
    
    MEM[address] = value;
    return;
}

