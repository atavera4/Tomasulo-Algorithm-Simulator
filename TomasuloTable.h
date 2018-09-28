//
//  TomasuloTable.h
//  CompDesign
//
//  Created by Ariel Taveras on 12/10/16.
//  Copyright © 2016 Ariel Taveras. All rights reserved.
//

#ifndef TomasuloTable_h
#define TomasuloTable_h


#endif /* TomasuloTable_h */


class TomasuloTable {
public:
    void LoadInstructionsToTable ();
    //[row][column]
    vector<string> instructions; // col #0
    vector<string> dest; // col #1
    vector<string> R1; // col #2
    vector<string> R2; // col #3
    vector<int> issue; // col #4
    vector<int> complete; // col #5
    vector<int> commit; // col #6
    
    vector<InstrType> instructions_issued;
    int instructions_issued_count;
    vector<string> instructions_issued_dest;
    vector<string> instructions_issued_R1;
    vector<string> instructions_issued_R2;
    int table_index;
    int total_instructions;
    int number_of_instr_issued;
    int instructions_completed;
    int System_Clock;
    bool table_complete;
    bool all_components_used;
    
    TomasuloTable() : table_complete(false), System_Clock(0), instructions_completed(0), instructions_issued_count(0), all_components_used(false), number_of_instr_issued(0), total_instructions(0),table_index(0) { ; }// no assignment needed.
}TomasuloTable;

void TomasuloTable::LoadInstructionsToTable () { // FIX THIS SO THAT INSTRUCTIONS ARE READ PROPERLY.
    ifstream input_file;
    input_file.open("Instructions.txt");
    string line;
    table_index = 0;
    if( input_file.is_open() ) {
        while( getline(input_file, line) ) {
            int i = 0;
            string temp;
            int commas_seen = 0;
            while(i < line.length()) {
                if (line[i] == ' ') {
                    instructions.push_back(temp);
                    temp = "";
                }
                else if (line[i] == ',') {
                    if (commas_seen == 0) {
                        dest.push_back(temp);
                    }
                    if (commas_seen == 1){
                        R1.push_back(temp);
                        R2.push_back(line.substr(i+1,2));
                    }
                    commas_seen++;
                    temp = "";
                }
                else if(line[i] == ('(')){
                    R1.push_back(temp);
                    R2.push_back(line.substr(i+1, 2));
                }
                else {
                    temp += line[i];
                }
                i++;
            }
            total_instructions++;
        }
    }
    else {
        cout << " FILE COULD NOT BE OPENED !!!" << endl;
    }
    cout << " LOADING TXT FILE COMPLETE" << endl;
    return;
}
