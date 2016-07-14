//
//  register.h
//  MIPS-Simulator
//
//  Created by 盛佩瑶 on 16/7/14.
//  Copyright © 2016年 盛佩瑶. All rights reserved.
//

#ifndef register_h
#define register_h
using namespace std;
struct Register{
    int regist[32];
    map<string, int> Reg;
    Register(){
        Reg["zero"] = Reg["0"] = 0;
        Reg["at"] = Reg["1"] = 1;
        Reg["v0"] = Reg["2"] = 2;
        Reg["v1"] = Reg["3"] = 3;
        Reg["a0"] = Reg["4"] = 4;
        Reg["a1"] = Reg["5"] = 5;
        Reg["a2"] = Reg["6"] = 6;
        Reg["a3"] = Reg["7"] = 7;
        Reg["t0"] = Reg["8"] = 8;
        Reg["t1"] = Reg["9"] = 9;
        Reg["t2"] = Reg["10"] = 10;
        Reg["t3"] = Reg["11"] = 11;
        Reg["t4"] = Reg["12"] = 12;
        Reg["t5"] = Reg["13"] = 13;
        Reg["t6"] = Reg["14"] = 14;
        Reg["t7"] = Reg["15"] = 15;
        Reg["s0"] = Reg["16"] = 16;
        Reg["s1"] = Reg["17"] = 17;
        Reg["s2"] = Reg["18"] = 18;
        Reg["s3"] = Reg["19"] = 19;
        Reg["s4"] = Reg["20"] = 20;
        Reg["s5"] = Reg["21"] = 21;
        Reg["s6"] = Reg["22"] = 22;
        Reg["s7"] = Reg["23"] = 23;
        Reg["t8"] = Reg["24"] = 24;
        
        Reg["t9"] = Reg["25"] = 25;
        Reg["sp"] = Reg["29"] = 29;
        Reg["fp"] = Reg["30"] = 30;
        Reg["ra"] = Reg["31"] = 31;
        regist[29] = 1000000;
    }
    int& operator [](string lab){
        if (Reg[lab] == 0) return regist[0] = 0;
        return regist[Reg[lab]];
    }
    int& operator [](int id){
        if (id == 0) return regist[0] = 0;
        return regist[id];
    }
    int get_id(string lab){
        return Reg[lab];
    }
};


#endif /* register_h */
