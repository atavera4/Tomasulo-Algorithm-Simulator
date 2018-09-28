//
//  Divider.h
//  CompDesign
//
//  Created by Ariel Taveras on 12/10/16.
//  Copyright © 2016 Ariel Taveras. All rights reserved.
//

#ifndef Divider_h
#define Divider_h
//#include "TomasuloTable.h"
//#include "Registers.h"
//#include "Memory.h"
//#include "ROB.h"
//#include "Loader.h"
//#include "Adder_Sub.h"
//#include "Multiplier.h"
////#include "Divider.h"
//#include "Storer.h"

#endif /* Divider_h */

class Div {
public:
    int Execute();
    void IssueFromReservationStation();
    //bool CheckForInputDependence(string R1, string R2);
    int execution_time;
    int time_elapsed;
    bool div_busy;
    string dest;
    float inputR1;
    float inputR2;
    int div_stations;
    bool has_ROB_token;
    deque<string> reservation_station_instr;
    deque<string> reservation_station_dest;
    deque<string> reservation_station_R1;
    deque<string> reservation_station_R2;
    float output;
    string ID;
    string Issue_ID;
}divider;

int Div::Execute() {
    if(time_elapsed >= execution_time) {
        output = inputR1 / inputR2;
        ROB.output_buffer.push_back(output);
        time_elapsed = 0;
        divider.div_busy = false;
        TomasuloTable.instructions_completed++;
        // take the instruction from the reservation station.
        reservation_station_instr.pop_front();
        reservation_station_dest.pop_front();
        reservation_station_R1.pop_front();
        reservation_station_R2.pop_front();
        div_stations++;
        return 1;
    }else { return 0; }
    cout << " Error in Div.Execute() function " << endl;
    return 0;
}
void Div::IssueFromReservationStation() {
    if(reservation_station_dest.size() <= 0) {
        return;
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
        IssueInstruction(DIV, dest, R1, R2);
    }
    return;
}
