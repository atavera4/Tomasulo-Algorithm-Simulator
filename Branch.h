//
//  Branch.h
//  CompDesign
//
//  Created by Ariel Taveras on 12/10/16.
//  Copyright © 2016 Ariel Taveras. All rights reserved.
//

#ifndef Branch_h
#define Branch_h


#endif /* Branch_h */


class Branch {
public:
    bool Execute();
    bool type; // TRUE== BEQ. FALSE== BNE.
    int offset;
    int input1;
    int input2;
    bool branch_busy;
    int branch_offset;
    int execution_time;
    int time_elapsed;
}branch;

bool Branch::Execute() {
    if(time_elapsed >= execution_time) {
        cout << "Branch has executed. moving back " << offset << " instructions" << endl;
        if(type == true) {
            if(input1 == input2) {
                branch_offset = offset;
            }
            else {
                branch_offset = 0;
            }
        }
        else {
            if(input1 != input2){
                branch_offset = offset;
            }
            else{
                branch_offset = 0;
            }
        }
        return true;
    }
    else { return false; }
    
    cout << "Error in Branch.Execute()" << endl;
    return false;
}
