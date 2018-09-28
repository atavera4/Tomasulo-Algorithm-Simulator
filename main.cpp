// Project for Computer Design
// Simulation of Tomasulos algorithm.
// Fill in Tomasulos algorithm table.
//Written by Ariel Taveras
// Fall 2016.

#include "header.h"
#include "Registers.h"
#include "TomasuloTable.h"
#include "ROB.h"
#include "Memory.h"
#include "Loader.h"
#include "Adder_Sub.h"
#include "Multiplier.h"
#include "Divider.h"
#include "Branch.h"
#include "Storer.h"

int main() {
    PromtUserForHardwareInfo();
    TomasuloTable.LoadInstructionsToTable();
    cout << "TOTAL INSTRUCTIONS == " << TomasuloTable.total_instructions << endl << endl;
    
    int i = 0;
    // when using branches just reset the num of instruction completed at the end of loop.
    while (TomasuloTable.instructions_completed < TomasuloTable.total_instructions) {

        // check to see if all instructions from file have been issued.
        if (TomasuloTable.number_of_instr_issued < TomasuloTable.total_instructions) {
            InstrType instr = GetInstructionEnum(TomasuloTable.instructions[i]);
            string dest = TomasuloTable.dest[i];
            string R1 = TomasuloTable.R1[i];
            string R2 = TomasuloTable.R2[i];
        
            // if the reservation stations are available then  issue. Else continue with execution and nothing else.
            if (LoadInstructionIntoReservationStation(instr, TomasuloTable.instructions[i], dest,R1, R2)) {
                i++;
            }
        }
        loader.IssueFromReservationStation();
        adder.IssueFromReservationStation();
        multiplier.IssueFromReservationStation();
        divider.IssueFromReservationStation();
        storer.IssueFromReservationStation();

        // execute the instructions that have been issued so far.
        ExecuteIssuedInstructions();
        
        // branch back to a certain instruction loaded in the sequence if branch has been used.
        if(branch.branch_offset > 0) {
            i = branch.branch_offset; // this will just be, i-0 = i , if no branch is used.
            TomasuloTable.number_of_instr_issued = i;
            TomasuloTable.instructions_completed = i;
            branch.branch_offset = 0;
        }
        
        
        ROB.CommitResults();
        TomasuloTable.System_Clock++;
    }
    Print_Registers_Memory();
    cout << endl << " END OF TOMASULO algorithm SIMULATOR " << endl;
    return 0;
}



InstrType GetInstructionEnum(string instr) {
    InstrType type;
    if (instr == "LD") {
        type = LD;
    }
    else if (instr == "ADD"){
        type = ADD;
    }
    else if (instr == "MULT") {
        type = MULT;
    }
    else if(instr == "DIV") {
        type = DIV;
    }
    else if (instr == "SD") {
        type = SD;
    }
    else if (instr == "ADDI") {
        type = ADDI;
    }
    else if (instr == "SUBI") {
        type = SUBI;
    }
    else if (instr == "BEQ") {
        type = BEQ;
    }
    else if (instr == "BNE") {
        type = BNE;
    }
    return type;
}

void IssueInstruction(InstrType instr, string dest, string R1, string R2) {
    string msg;
    switch (instr) {
        case LD:
            if(loader.Loader_busy) { return; }
             msg = "LD has begun execution at time " + to_string(TomasuloTable.System_Clock);
            cout << "LD has begun execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
            PrintOut.list.push_front(msg);
            loader.Loader_busy = true;
            loader.output = Memory.GetDataFromMem(R1, R2);
            loader.dest = dest;
            TomasuloTable.instructions_issued.push_back(LD);
            TomasuloTable.instructions_issued_count++;
            // Take control of the destination register for reservation station and ROB purposes.
            Registers.TakeRegToken(dest);
            loader.has_ROB_token = Registers.TakeROB_RegToken(dest);
            break;
        case ADD:
            if(adder.adder_busy) { return; }
             msg = "ADD has begun execution at time " + to_string(TomasuloTable.System_Clock);
            cout << "ADD has begun execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
            PrintOut.list.push_front(msg);
            adder.adder_busy = true;
            adder.immediate_op = false;
            adder.add_or_sub_OP = true; // TRUE indicates ADDITION.
            adder.inputR1 = Registers.GetDataFromReg(R1);
            adder.inputR2 = Registers.GetDataFromReg(R2);
            adder.dest = dest;
            TomasuloTable.instructions_issued.push_back(ADD);
            TomasuloTable.instructions_issued_count++;
            // Take control of the destination register for reservation station purposes. Prevents a future instruction that reads from this instructions dest register from issuing until this instruction has completed and written to that register.
            Registers.TakeRegToken(dest);
            adder.has_ROB_token = Registers.TakeROB_RegToken(dest);
            break;

        case SUB:
            if(adder.adder_busy) { return; }
            msg = "SUB has begun execution at time " + to_string(TomasuloTable.System_Clock);
            cout << "SUB has begun execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
            PrintOut.list.push_front(msg);
            adder.adder_busy = true;
            adder.add_or_sub_OP = false;
            adder.immediate_op = false;
            adder.inputR1 = Registers.GetDataFromReg(R1);
            adder.inputR2 = Registers.GetDataFromReg(R2);
            adder.dest = dest;
            TomasuloTable.instructions_issued.push_back(ADD);
            TomasuloTable.instructions_issued_count++;
            // Take control of the destination register for reservation station and ROB purposes.
            Registers.TakeRegToken(dest);
            adder.has_ROB_token = Registers.TakeROB_RegToken(dest);
            break;

        case MULT:
            if (multiplier.mult_busy) { return; }
            msg = "MULT has begun execution at time " + to_string(TomasuloTable.System_Clock);
            cout << "MULT has begun execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
            PrintOut.list.push_front(msg);
            multiplier.mult_busy = true;
            multiplier.inputR1 = Registers.GetDataFromReg(R1);
            multiplier.inputR2 = Registers.GetDataFromReg(R2);
            multiplier.dest = dest;
            TomasuloTable.instructions_issued.push_back(MULT);
            TomasuloTable.instructions_issued_count++;
            Registers.TakeRegToken(dest);
            multiplier.has_ROB_token = Registers.TakeROB_RegToken(dest);
            break;
        case DIV:
            if(divider.div_busy) { return; }
            msg = "DIV has begun execution at time " + to_string(TomasuloTable.System_Clock);
            cout << "DIV has begun execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
            PrintOut.list.push_front(msg);
            divider.div_busy = true;
            divider.inputR1 = Registers.GetDataFromReg(R1);
            divider.inputR2 = Registers.GetDataFromReg(R2);
            divider.dest = dest;
            TomasuloTable.instructions_issued.push_back(DIV);
            TomasuloTable.instructions_issued_count++;
            Registers.TakeRegToken(dest);
            divider.has_ROB_token = Registers.TakeROB_RegToken(dest);
            break;
        case SD:
            if(storer.store_busy) { return; }
            msg = "SD has begun execution at time " + to_string(TomasuloTable.System_Clock);
            cout << "SD has begun execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
            PrintOut.list.push_front(msg);
            storer.store_busy = true;
            // here the dest is the source register with the value to be stored.
            storer.source_reg_value = Registers.GetDataFromReg(dest);
            storer.store_address = Memory.GetAddress(R1, R2);
            TomasuloTable.instructions_issued.push_back(SD);
            TomasuloTable.instructions_issued_count++;
//            Registers.TakeRegToken(dest);
//            storer.has_ROB_token = Registers.TakeROB_RegToken(dest);
            break;
        case ADDI:
            if(adder.adder_busy) { return; }
            msg = "ADDI has begun execution at time " + to_string(TomasuloTable.System_Clock);
            cout << "ADDI has begun execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
            PrintOut.list.push_front(msg);
            adder.adder_busy = true;
            adder.add_or_sub_OP = true; // TRUE indicates ADDITION.
            adder.inputR1 = Registers.GetDataFromReg(R1);
            adder.inputR2 = stoi(R2.erase(0,1)); // load the immediate.
            adder.dest = dest;
            adder.immediate_op = true;
            TomasuloTable.instructions_issued.push_back(ADDI);
            TomasuloTable.instructions_issued_count++;
            // Take control of the destination register for reservation station purposes. Prevents a future instruction that reads from this instructions dest register from issuing until this instruction has completed and written to that register.
            Registers.TakeRegToken(dest);
            adder.has_ROB_token = Registers.TakeROB_RegToken(dest);
            break;
        case SUBI:
            if(adder.adder_busy) { return; }
            msg = "SUBI has begun execution at time " + to_string(TomasuloTable.System_Clock);
            cout << "SUBI has begun execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
            PrintOut.list.push_front(msg);
            adder.adder_busy = true;
            adder.immediate_op = true;
            adder.add_or_sub_OP = false; // false indicates substraction.
            adder.inputR1 = Registers.GetDataFromReg(R1);
            adder.inputR2 = stoi(R2.erase(0,1)); // load the immediate.
            adder.dest = dest;
            TomasuloTable.instructions_issued.push_back(SUBI);
            TomasuloTable.instructions_issued_count++;
            // Take control of the destination register for reservation station and ROB purposes.
            Registers.TakeRegToken(dest);
            adder.has_ROB_token = Registers.TakeROB_RegToken(dest);
            break;
        
        default:
            //cout << " Issue Instruction Function HIT DEFAULT !!!" << endl;
            break;
    }
    return;
}

void ExecuteIssuedInstructions() {
    // check if instruction has been issued.
    // check if its execution time has passed.
    // UPDATE ELAPSED TIME FOR THE HARDWARE PARTS ISSUED SO FAR.
    for(int i =0; i < TomasuloTable.instructions_issued_count;) {
        string msg;
        switch (TomasuloTable.instructions_issued[i]) {
            case LD:
                if(loader.Execute()) {
                    TomasuloTable.instructions_issued.erase(TomasuloTable.instructions_issued.begin() + i);
                    TomasuloTable.instructions_issued_count--;
                    msg = "LD has completed execution at time " + to_string(TomasuloTable.System_Clock);
                    cout << "LD has completed execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
                    cout << "LD Writeback at time " + to_string(TomasuloTable.System_Clock+1) << endl;
                    PrintOut.list.push_front(msg);
                }
                else {
                    loader.time_elapsed++;
                    i++;
                }
                break;
            case ADD:
                if(adder.Execute()) {
                    TomasuloTable.instructions_issued.erase(TomasuloTable.instructions_issued.begin() + i);
                    TomasuloTable.instructions_issued_count--;
                    msg = "ADD has completed execution at time " + to_string(TomasuloTable.System_Clock);
                    cout << "ADD has completed execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
                    PrintOut.list.push_front(msg);
                    cout << "ADD Writeback at time " + to_string(TomasuloTable.System_Clock+1) << endl;
                }
                else {
                    adder.time_elapsed++;
                    i++;
                }
                break;
            case SUB:
                if(adder.Execute()) {
                    TomasuloTable.instructions_issued.erase(TomasuloTable.instructions_issued.begin() + i);
                    TomasuloTable.instructions_issued_count--;
                    msg = "SUB has completed execution at time " + to_string(TomasuloTable.System_Clock);
                    cout << "SUB has completed execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
                    PrintOut.list.push_front(msg);
                    cout << "SUB Writeback at time " + to_string(TomasuloTable.System_Clock+1) << endl;

                }
                else {
                    adder.time_elapsed++;
                    i++;
                }
                break;
            case MULT:
                if(multiplier.Execute()) {
                    TomasuloTable.instructions_issued.erase(TomasuloTable.instructions_issued.begin() + i);
                    TomasuloTable.instructions_issued_count--;
                    msg = "MULT has completed execution at time " + to_string(TomasuloTable.System_Clock);
                    cout << "MULT has completed execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
                    PrintOut.list.push_front(msg);
                    cout << "MULT Writeback at time " + to_string(TomasuloTable.System_Clock+1) << endl;
                }
                else {
                    multiplier.time_elapsed++;
                    i++;
                }
                break;
            case DIV:
                if(divider.Execute()) {
                    TomasuloTable.instructions_issued.erase(TomasuloTable.instructions_issued.begin() + i);
                    TomasuloTable.instructions_issued_count--;
                    msg = "DIV has completed execution at time " + to_string(TomasuloTable.System_Clock);
                    cout << "DIV has completed execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
                    PrintOut.list.push_front(msg);
                    cout << "DIV Writeback at time " + to_string(TomasuloTable.System_Clock+1) << endl;
                }
                else {
                    divider.time_elapsed++;
                    i++;
                }
                break;
            case SD:
                if(storer.Execute()) {
                    TomasuloTable.instructions_issued.erase(TomasuloTable.instructions_issued.begin() + i);
                    TomasuloTable.instructions_issued_count--;
                    msg = "SD has completed execution at time " + to_string(TomasuloTable.System_Clock);
                    cout << "SD has completed execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
                    PrintOut.list.push_front(msg);
                    cout << "SD Writeback at time " + to_string(TomasuloTable.System_Clock+1) << endl;
                }
                else{
                    storer.time_elapsed++;
                    i++;
                }
                break;
            case ADDI:
                if(adder.Execute()) {
                    TomasuloTable.instructions_issued.erase(TomasuloTable.instructions_issued.begin() + i);
                    TomasuloTable.instructions_issued_count--;
                    msg = "ADDI has completed execution at time " + to_string(TomasuloTable.System_Clock);
                    cout << "ADDI has completed execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
                    PrintOut.list.push_front(msg);
                    cout << "ADDI Writeback at time " + to_string(TomasuloTable.System_Clock+1) << endl;
                    
                }
                else {
                    adder.time_elapsed++;
                    i++;
                }
                break;
            case SUBI:
                if(adder.Execute()) {
                    TomasuloTable.instructions_issued.erase(TomasuloTable.instructions_issued.begin() + i);
                    TomasuloTable.instructions_issued_count--;
                    msg = "SUBI has completed execution at time " + to_string(TomasuloTable.System_Clock);
                    cout << "SUBI has completed execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
                    PrintOut.list.push_front(msg);
                    cout << "SUBI Writeback at time " + to_string(TomasuloTable.System_Clock+1) << endl;

                }
                else {
                    adder.time_elapsed++;
                    i++;
                }
                break;
            case BEQ:
                if(branch.Execute()) {
                    branch.branch_busy = false;
                    TomasuloTable.instructions_issued.erase(TomasuloTable.instructions_issued.begin() + i);
                    TomasuloTable.instructions_issued_count--;
                    TomasuloTable.instructions_completed++;
                    msg = "BEQ has completed execution at time " + to_string(TomasuloTable.System_Clock);
                    cout << "BEQ has completed execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
                    PrintOut.list.push_front(msg);

                }
                else {
                    branch.time_elapsed++;
                    i++;
                }
                break;
            case BNE:
                if(branch.Execute()) {
                    branch.branch_busy = false;
                    TomasuloTable.instructions_issued.erase(TomasuloTable.instructions_issued.begin() + i);
                    TomasuloTable.instructions_issued_count--;
                    TomasuloTable.instructions_completed++;
                    msg = "BNE has completed execution at time " + to_string(TomasuloTable.System_Clock);
                    cout << "BNE has completed execution at time  " + to_string(TomasuloTable.System_Clock) << endl;
                    PrintOut.list.push_front(msg);
                }
                else {
                    branch.time_elapsed++;
                    i++;
                }
                break;
                
            default:
                cout << "Execute Issue funct not WOrKING !!!!" << endl;
                break;
        }
    }
    return;
}

bool LoadInstructionIntoReservationStation(InstrType type, string instr, string dest, string R1, string R2) {
    string msg;
    switch(type) {
        case LD:
            if (loader.loader_stations == 0) {
                return false;
            }
            else{
                msg = "Putting LD in RESERV Station at time " + to_string(TomasuloTable.System_Clock);
                cout << "Putting LD in RESERV Station at time " + to_string(TomasuloTable.System_Clock) << endl;
                PrintOut.list.push_front(msg);
                loader.reservation_station_instr.push_back(instr);
                loader.reservation_station_dest.push_back(dest);
                loader.reservation_station_R1.push_back(R1);
                loader.reservation_station_R2.push_back(R2);
                loader.loader_stations--; // subtract from total number of available reservation stations for loader.
                TomasuloTable.number_of_instr_issued++;
                return true;
            }
            break;
        case ADD:
            if (adder.add_stations == 0) {
                return false;
            }
            else {
                msg = "Putting LD in RESERV Station at time " + to_string(TomasuloTable.System_Clock);
                cout << "Putting ADD in RESERV Station at time " + to_string(TomasuloTable.System_Clock) << endl;
                PrintOut.list.push_front(msg);
                adder.reservation_station_instr.push_back(instr);
                adder.reservation_station_dest.push_back(dest);
                adder.reservation_station_R1.push_back(R1);
                adder.reservation_station_R2.push_back(R2);
                adder.add_stations--;
                TomasuloTable.number_of_instr_issued++;
                return true;
            }
            break;
        case SUB:
            if (adder.add_stations == 0) {
                return false;
            }
            else {
                msg = "Putting SUB in RESERV Station at time " + to_string(TomasuloTable.System_Clock);
                cout << "Putting SUB in RESERV Station at time " + to_string(TomasuloTable.System_Clock) << endl;
                PrintOut.list.push_front(msg);
                adder.reservation_station_instr.push_back(instr);
                adder.reservation_station_dest.push_back(dest);
                adder.reservation_station_R1.push_back(R1);
                adder.reservation_station_R2.push_back(R2);
                adder.add_stations--;
                TomasuloTable.number_of_instr_issued++;
                return true;
            }
            break;
        case MULT:
            if(multiplier.mult_stations == 0) {
                return false;
            }
            else {
                msg = "Putting MULT in RESERV Station at time " + to_string(TomasuloTable.System_Clock);
                cout << "Putting MULT in RESERV Station at time " + to_string(TomasuloTable.System_Clock) << endl;
                PrintOut.list.push_front(msg);
                multiplier.reservation_station_instr.push_back(instr);
                multiplier.reservation_station_dest.push_back(dest);
                multiplier.reservation_station_R1.push_back(R1);
                multiplier.reservation_station_R2.push_back(R2);
                multiplier.mult_stations--;
                TomasuloTable.number_of_instr_issued++;
                return true;
            }
            break;
            
        case DIV:
            if (divider.div_stations == 0) {
                return false;
            }
            else {
                msg = "Putting DIV in RESERV Station at time " + to_string(TomasuloTable.System_Clock);
                cout << "Putting DIV in RESERV Station at time " + to_string(TomasuloTable.System_Clock) << endl;
                PrintOut.list.push_front(msg);
                divider.reservation_station_instr.push_back(instr);
                divider.reservation_station_dest.push_back(dest);
                divider.reservation_station_R1.push_back(R1);
                divider.reservation_station_R2.push_back(R2);
                divider.div_stations--;
                TomasuloTable.number_of_instr_issued++;
                return true;
            }
            break;
        case SD:
            if (storer.storer_stations == 0) {
                return false;
            }
            else {
                msg = "Putting SD in RESERV Station at time " + to_string(TomasuloTable.System_Clock);
                cout << "Putting SD in RESERV Station at time " + to_string(TomasuloTable.System_Clock) << endl;
                PrintOut.list.push_front(msg);
                storer.reservation_station_instr.push_back(instr);
                storer.reservation_station_dest.push_back(dest);
                storer.reservation_station_R1.push_back(R1);
                storer.reservation_station_R2.push_back(R2);
                storer.storer_stations--;
                TomasuloTable.number_of_instr_issued++;
                return true;
            }
        case ADDI:
            if (adder.add_stations == 0) {
                return false;
            }
            else {
                msg = "Putting ADDI in RESERV Station at time " + to_string(TomasuloTable.System_Clock);
                cout << "Putting ADDI in RESERV Station at time " + to_string(TomasuloTable.System_Clock) << endl;
                PrintOut.list.push_front(msg);
                adder.reservation_station_instr.push_back(instr);
                adder.reservation_station_dest.push_back(dest);
                adder.reservation_station_R1.push_back(R1);
                adder.reservation_station_R2.push_back(R2);
                adder.add_stations--;
                TomasuloTable.number_of_instr_issued++;
                return true;
            }
            break;
        case SUBI:
            if (adder.add_stations == 0) {
                return false;
            }
            else {
                msg = "Putting SUBI in RESERV Station at time " + to_string(TomasuloTable.System_Clock);
                cout << "Putting SUBI in RESERV Station at time " + to_string(TomasuloTable.System_Clock) << endl;
                PrintOut.list.push_front(msg);
                adder.reservation_station_instr.push_back(instr);
                adder.reservation_station_dest.push_back(dest);
                adder.reservation_station_R1.push_back(R1);
                adder.reservation_station_R2.push_back(R2);
                adder.add_stations--;
                TomasuloTable.number_of_instr_issued++;
                return true;
            }
            break;
        case BEQ:
            msg = "BEQ branch has been issued at time  " + to_string(TomasuloTable.System_Clock);
            cout << "BEQ branch has been issued at time   " + to_string(TomasuloTable.System_Clock) << endl;
            PrintOut.list.push_front(msg);
            if(branch.branch_busy) { return false; }
            branch.branch_busy = true;
            branch.type = true; // TRUE = BEQ.
            branch.offset =  stoi(R2);
            branch.input1 = Registers.GetDataFromReg(dest);
            branch.input2 = Registers.GetDataFromReg(R1);
            TomasuloTable.instructions_issued.push_back(BEQ);
            TomasuloTable.instructions_issued_count++;
            TomasuloTable.number_of_instr_issued++;
            return true;
            break;
            
        case BNE:
            msg = "BEQ branch has been issued at time  " + to_string(TomasuloTable.System_Clock);
            cout << "BNE branch has been issued at time   " + to_string(TomasuloTable.System_Clock) << endl;
            PrintOut.list.push_front(msg);
            if(branch.branch_busy) { return false; }
            branch.branch_busy = true;
            branch.type = false; // TRUE = BEQ.
            branch.offset = stoi(R2);
            branch.input1 = Registers.GetDataFromReg(dest);
            branch.input2 = Registers.GetDataFromReg(R1);
            TomasuloTable.instructions_issued.push_back(BNE);
            TomasuloTable.instructions_issued_count++;
            TomasuloTable.number_of_instr_issued++;
            return true;
            break;


        default:
            cout << " RESERVATION FUNCTION DOES NOT WORK PROPERLY" << endl;
    }
    return false;
}

// Check to see if a previous/recently issueD instruction that has not completed needs to write to the register that you need for input.
bool CheckForInputDependence(string R1, string R2) {
    bool result;
    if(R2 == "no input"){
        result = Registers.CheckIfAvailable(R1, "no input");
    }
    result = Registers.CheckIfAvailable(R1, R2);
    return result;
}


void Print_Registers_Memory() {
    cout << endl << endl;
    
    for(int i=0; i < 32; i++) {
        cout << "FP Register " + to_string(i) + " value = " << Registers.REG[i] << endl;
    }
    for(int i=0; i < 18; i++) {
        cout << "Memory unit " + to_string(i) + " value = " << Memory.MEM[i] << endl;
    }
    for(int i =0; i < 32; i++) {
        cout << "Integer Register " + to_string(i) + " value = " << Registers.Int_REG[i] << endl;
    }
}


void PromtUserForHardwareInfo() {
    
    cout << "Enter the number of cycles for the Load Operation to complete" << endl;
    cin >> loader.execution_time;
    cout << "Enter the number of Reservation Stations for the Loader" << endl;
    cin >> loader.loader_stations;
    
    cout << "Enter the number of cycles for the STORE Operation to complete" << endl;
    cin >> storer.execution_time;
    cout << "Enter the number of Reservation Stations for the Store Unit" << endl;
    cin >> storer.storer_stations;
    
    cout << "enter the number of cycles for the Adder/subtract Unit" << endl;
    cin >> adder.execution_time;
    cout << " Enter the number of Reservation Stations for the Adder/Subtractor Unit" << endl;
    cin >> adder.add_stations;
    
    cout << "enter the number of cycles for the Multiply Operation to complete" << endl;
    cin >> multiplier.execution_time;
    cout << "Enter the Number of Reservation Stations for the MULTIPLY Unit" << endl;
    cin >> multiplier.mult_stations;
    
    cout << "enter the number of cycles for the Divider Operation to complete" << endl;
    cin >> divider.execution_time;
    cout << "Enter the number of Reservation Stations for the Divider Unit" << endl;
    cin >> divider.div_stations;
    
    cout << " Enter the number of cycles for the Branch Instructions to complete" << endl;
    cin >> branch.execution_time;
    return;
}
