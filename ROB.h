//
//  ROB.h
//  CompDesign
//
//  Created by Ariel Taveras on 12/10/16.
//  Copyright © 2016 Ariel Taveras. All rights reserved.
//

#ifndef ROB_h
#define ROB_h


#endif /* ROB_h */


class ReOrderBuffer { // ???? figure out how this works before continuing !!!!!
public:
    void CommitResults();
    deque<string> instruction_buffer;
    deque<string> output_destination_buffer;
    deque<string> Destination_REG_queue;
    deque<int> output_buffer;
    deque<bool> Register_Tokens;
    
}ROB;

// Array of queues.
// each register will have a single token that
//      allows an instruction holding it to write to that register.
// if an instruction needs the reg and token is available it will take its token.
// else it will be placed in the queue. with all its data.
// once token is released next instruction in ROB can take token and proceed to commit.
// void ReOrderBuffer:: TakeToken(string dest_reg) {
//   //
//   return;
// }

void ReOrderBuffer::CommitResults() {
    
    // check to see if the ROB is empty.
    if (output_destination_buffer.size() <= 0) {
        return;
    }
    int CommitedInstructions = 0;
    int size = instruction_buffer.size();
    //iterate through vector and look for anything with an ROB commit token.
    // once found commit its value and release the token for that register.
    string dest;
    string msg;
    string instr;
    for(int i = 0; i < size  ; i++) {
        if (Register_Tokens[i]) {
            dest = output_destination_buffer[i];
            Registers.StoreValueInReg(dest, output_buffer[i]);
            Registers.ReleaseROB_RegToken(dest);
            Registers.ReleaseRegToken(dest);
            instr = instruction_buffer[i] + " ";// + output_destination_buffer[i];
            msg = instr + "  Commited at time " + to_string(TomasuloTable.System_Clock +1);
            cout << instr + "  Commited at time  " + to_string(TomasuloTable.System_Clock+1) << endl;
            PrintOut.list.push_front(msg);
            CommitedInstructions++;
        }
        else {
            // stop b/c the current instruction being checked in the ROB is not ready to be commited yet.
            break;
        }
    }
    // remove or pop the commited instructions and their data from ROB queue.
    for(int i=0 ; i < CommitedInstructions; i++) {
        Register_Tokens.pop_front();
        instruction_buffer.pop_front();
        output_buffer.pop_front();
        output_destination_buffer.pop_front();
    }
    // then look for the next instruction in the queue that needs that registers token and assign it.
    // exit the function. it will repeat in next system clock iteration.
    
    size = instruction_buffer.size();
    for(int i = 0; i  < size; i++ ) {
        if(Register_Tokens[i] == false){
            string dest_reg = output_destination_buffer[i];
            Registers.TakeROB_RegToken(dest_reg);
        }
    }
    return;
}
