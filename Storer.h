//
//  Storer.h
//  CompDesign
//
//  Created by Ariel Taveras on 12/10/16.
//  Copyright © 2016 Ariel Taveras. All rights reserved.
//

#ifndef Storer_h
#define Storer_h

#endif /* Storer_h */

class Store {
public:
    int Execute();
    void IssueFromReservationStation();
    //  bool CheckForInputDependence(string R1, string R2);
    int execution_time;
    int time_elapsed;
    bool store_busy;
    string dest;
    int source_reg_value;
    int store_address;
    int storer_stations;
    bool has_ROB_token;
    deque<string> reservation_station_instr;
    deque<string> reservation_station_dest;
    deque<string> reservation_station_R1;
    deque<string> reservation_station_R2;
    int output;
    string ID;
    string Issue_ID;
}storer;

int Store::Execute() {
    if (time_elapsed >= execution_time) {
        Memory.StoreValueInMem(store_address,source_reg_value);
        time_elapsed = 0;
        storer.store_busy = false;
        TomasuloTable.instructions_completed++;
        // take the instruction from the reservation station.
        reservation_station_instr.pop_front();
        reservation_station_dest.pop_front();
        reservation_station_R1.pop_front();
        reservation_station_R2.pop_front();
        storer_stations++;
        return 1;
    }else { return 0; }
    
    cout << " Error in Storer.Execute() " << endl;
    return 0;
}

void Store::IssueFromReservationStation() {
    if(reservation_station_dest.size() <= 0){
        return; // do nothing if the reservation stations all empty.
    }
    string R1 = reservation_station_R1[reservation_station_R1.size()-1];
    string R2 = reservation_station_R2[reservation_station_R2.size()-1];
    string dest = reservation_station_dest[reservation_station_dest.size()-1];
    if (CheckForInputDependence(dest, "no input")) {
        // true means that all inputs are not ready.
        return;
    }
    else {
        IssueInstruction(SD, dest, R1, R2);
    }
    return;
}

