//
//  Adder_Sub.h
//  CompDesign
//
//  Created by Ariel Taveras on 12/10/16.
//  Copyright © 2016 Ariel Taveras. All rights reserved.
//

#ifndef Adder_Sub_h
#define Adder_Sub_h


#endif /* Adder_Sub_h */
//#include "TomasuloTable.h"
//#include "Registers.h"
//#include "Memory.h"
//#include "ROB.h"
//#include "Loader.h"
////#include "Adder_Sub.h"
//#include "Multiplier.h"
//#include "Divider.h"
//#include "Storer.h"

class Adder {
public:
    int Execute();
    void IssueFromReservationStation();
    //  bool CheckForInputDependence(string R1, string R2);
    int execution_time;
    int time_elapsed;
    bool add_or_sub_OP; // if true then it is an ADD operation. false= sub.
    bool adder_busy;
    bool immediate_op; // TRUE == immediate operation.
    string dest;
    int inputR1;
    int inputR2;
    float output;
    int add_stations;
    bool has_ROB_token;
    deque<string> reservation_station_instr;
    deque<string> reservation_station_dest;
    deque<string> reservation_station_R1;
    deque<string> reservation_station_R2;
    string ID;
    string Issue_ID;
}adder;


int Adder::Execute() {
    if (time_elapsed >= execution_time) {
        // determine if the operation is addition or subtraction first.
        if (add_or_sub_OP) {
            output = inputR1 + inputR2;
        }
        else {
            output = inputR1 - inputR2;
        }
        
        // Send result to the ROB buffer.
        ROB.output_buffer.push_back(output);
        if (immediate_op) {
            ROB.instruction_buffer.push_back("ADDI");
        }
        else {
            ROB.instruction_buffer.push_back("ADD");
        }
        ROB.output_destination_buffer.push_back(dest);
        ROB.Register_Tokens.push_back(has_ROB_token);
        time_elapsed = 0;
        adder.adder_busy = false;
        TomasuloTable.instructions_completed++;
        // take the instruction from the reservation station.
        reservation_station_instr.pop_front();
        reservation_station_dest.pop_front();
        reservation_station_R1.pop_front();
        reservation_station_R2.pop_front();
        // release the reservation station register token.
        add_stations++;
        return 1;
    }else { return 0; }
    
    cout << "Error in Adder.Execute() function !!!!" << endl;
    return 0;
}

void Adder::IssueFromReservationStation() {
    if(reservation_station_dest.size() <= 0){
        return; // do nothing if the reservation stations all empty.
    }
    string instruction = reservation_station_instr[0];
    string R1 = reservation_station_R1[0];
    string R2 = reservation_station_R2[0];
    string dest = reservation_station_dest[0];
    if (CheckForInputDependence(R1, R2)) {
        // true means that all inputs are not ready.
        
        return;
    }
    else {
        if (instruction == "ADD"){
            IssueInstruction(ADD, dest, R1, R2);
        }
        else if (instruction == "SUB") {
            IssueInstruction(SUB, dest, R1, R2);
        }
        else if(instruction == "ADDI") {
            IssueInstruction(ADDI, dest, R1, R2);
        }
        else if (instruction == "SUBI") {
            IssueInstruction(SUBI, dest, R1, R2);
        }
    }
    return; 
}
