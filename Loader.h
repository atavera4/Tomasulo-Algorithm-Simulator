//
//  Loader.h
//  CompDesign
//
//  Created by Ariel Taveras on 12/10/16.
//  Copyright © 2016 Ariel Taveras. All rights reserved.
//

#ifndef Loader_h
#define Loader_h


#endif /* Loader_h */
//#include "TomasuloTable.h"
//#include "Registers.h"
////#include "Memory.h"
//#include "ROB.h"
//#include "Loader.h"
//#include "Adder_Sub.h"
//#include "Multiplier.h"
//#include "Divider.h"
//#include "Storer.h"

class Loader {
public:
    void UpdateElapsedTime();
    void TakeInNewData(string dest, string inputR1, string inputR2);
    void IssueFromReservationStation();
    //bool CheckForInputDependence(string R1, string R2);
    int Execute();
    int execution_time;
    int time_elapsed;
    bool Loader_busy;
    bool has_ROB_token;
    string dest;
    string inputR1;
    string inputR2;
    int output;
    int loader_stations;
    deque<string> reservation_station_instr;
    deque<string> reservation_station_dest;
    deque<string> reservation_station_R1;
    deque<string> reservation_station_R2;
    string ID;
    string Issue_ID;
};
Loader loader;

void Loader::TakeInNewData(string dest, string inputR1, string inputR2){
    this->dest = dest;
    this->inputR1 = inputR1;
    this->inputR2 = inputR2;
    return;
}
int Loader::Execute() {
    if (time_elapsed >= execution_time) {
        // Registers.StoreValueInReg(dest, output); // send result to ROB with all info.
        
        ROB.output_buffer.push_back(output);
        ROB.instruction_buffer.push_back("LD");
        ROB.output_destination_buffer.push_back(dest);
        ROB.Register_Tokens.push_back(has_ROB_token);
        cout << "LOADER FINISHED AT time " << time_elapsed << endl;
        time_elapsed = 0;
        Loader_busy = false;
        TomasuloTable.instructions_completed++;
        // take the instruction from the reservation station.
        reservation_station_instr.pop_front();
        reservation_station_dest.pop_front();
        reservation_station_R1.pop_front();
        reservation_station_R2.pop_front();
        loader_stations++;
        return 1;
    } else { return 0; }
    
    cout << "Error in Loader.Execute funct !!!!!" << endl;
    return 0;
}

void Loader::IssueFromReservationStation() {
    if(reservation_station_dest.size() <= 0){
        return; // do nothing if the reservation stations all empty.
    }
    string R1 = reservation_station_R1[reservation_station_R1.size()-1];
    string R2 = reservation_station_R2[reservation_station_R2.size()-1];
    string dest = reservation_station_dest[reservation_station_dest.size()-1];
//    if (CheckForInputDependence(R1, R2)) {
//        // true means that all inputs are not ready.
//        return;
//    }
//    else {
//        cout << " In Loader Station" << endl;
//        IssueInstruction(LD, dest, R1, R2);
//    }
    IssueInstruction(LD, dest, R1, R2);
    return;
}
