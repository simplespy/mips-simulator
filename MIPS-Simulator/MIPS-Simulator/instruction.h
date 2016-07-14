//
//  instruction.h
//  MIPS-Simulator
//
//  Created by 盛佩瑶 on 16/7/14.
//  Copyright © 2016年 盛佩瑶. All rights reserved.
//

#ifndef instruction_h
#define instruction_h
using namespace std;
struct Instruction{
    map<string, int> ID;
    Instruction(){
        ID["syscall"] = 0;
        ID["li"] = 1;
        ID["la"] = 2;
        ID["lb"] = 3;
        ID["lw"] = 4;
        ID["sb"] = 5;
        ID["sh"] = 6;
        ID["sw"] = 7;
        ID["sd"] = 8;
        ID["blt"] = 9;
        ID["ble"] = 10;
        ID["bgt"] = 11;
        ID["bge"] = 12;
        ID["beq"] = 13;
        ID["bne"] = 14;
        ID["add"] = 15;
        ID["addu"] = 15;
        ID["mul"] = 16;
        ID["sub"] = 17;
        ID["subu"] = 17;
        ID["div"] = 18;
        ID["rem"] = 19;
        ID["slt"] = 20;
        ID["sle"] = 21;
        ID["sgt"] = 22;
        ID["sge"] = 23;
        ID["seq"] = 24;
        ID["move"] = 25;
        ID["neg"] = 26;
        ID["jr"] = 27;
        ID["mflo"] = 28;
        ID["mfhi"] = 29;
        ID["jal"] = 30;
        ID["b"] = 31;
        ID["j"] = 32;
        ID["bgtz"] = 33;
        ID["bgez"] = 34;
        ID["beqz"] = 35;
        ID["bnez"] = 36;
        ID["xor"] = 37;
        ID["sne"] = 38;
        
    }
    int operator [](char ins[]){
        return ID[ins];
    }

};


#endif /* instruction_h */
