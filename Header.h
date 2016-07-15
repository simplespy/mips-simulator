//
//  Header.h
//  MIPS-Simulator
//
//  Created by 盛佩瑶 on 16/7/15.
//  Copyright © 2016年 盛佩瑶. All rights reserved.
//

#ifndef Header_h
#define Header_h
using namespace std;
const int type[] = {0,1,2,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,6,6,6,7,7,7,8,8,8,8,4,4};

struct instruction{
    int id;
    int par[3];
    int offset;
    int number[3];
    char la[3][100];
    int rid[3];
    instruction(){
        for (int i = 0; i < 3; ++ i){
            par[i] = number[i] = rid[i] = 0;
        }
        offset = id = 0;
    }
};

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

void fun00(fstream &fin);
void fun0(int i){
    cout << "I am a salted fish " << endl;
}
void fun1(int i);
void fun2(int i);
void fun3(int i);
void fun4(int i);
void fun5(int i);
void fun6(int i);
void fun7(int i);
void fun8(int i);
void fun9(int i);
void fun10(int i);
void fun11(int i);
void fun12(int i);
void fun13(int i);
void fun14(int i);
void fun15(int i);
void fun16(int i);
void fun17(int i);
void fun18(int i);
void fun19(int i);
void fun20(int i);
void fun21(int i);
void fun22(int i);
void fun23(int i);
void fun24(int i);
void fun25(int i);
void fun26(int i);
void fun27(int i);
void fun28(int i);
void fun29(int i);
void fun30(int i);
void fun31(int i);
void fun32(int i);
void fun33(int i);
void fun34(int i);
void fun35(int i);
void fun36(int i);
void fun37(int i);
void fun38(int i);

void (*f[])(int i) = {fun0, fun1, fun2, fun3, fun4, fun5, fun6, fun7, fun8, fun9, fun10,fun11, fun12, fun13, fun14, fun15, fun16, fun17, fun18, fun19, fun20, fun21, fun22, fun23, fun24, fun25, fun26, fun27, fun28, fun29, fun30, fun31, fun32, fun33, fun34, fun35, fun36, fun37, fun38};

void extract_label (char (&in)[100], char (&r)[100], int begin){
    memset(r,0,sizeof(r));
    int len = 0;
    for (int i = begin; i < strlen(in); ++ i){
        if ((in[i] >= 'a' && in[i] <= 'z') ||
            (in[i] >= 'A' && in[i] <= 'Z') ||
            (in[i] >= '0' && in[i] <= '9') ||
            (in[i] == '_'))
            r[len++] = in[i];
    }
    r[len] = '\0';
}

bool extract_ins(char (&a)[100], char (&b)[100], int & i){
    memset(b,0,sizeof(b));
    int len = 0;
    while (!((a[i] >= 'a' && a[i] <= 'z') ||
             (a[i] >= 'A' && a[i] <= 'Z') ||
             (a[i] >= '0' && a[i] <= '9') ||
             (a[i] == '_'))) ++ i;
    for (; i < strlen(a); ++ i){
        if (a[i] == ' ' || a[i] == ':') break;
        else b[len++] = a[i];
    }
    b[len] = '\0';
    if (a[i] == ':') return false;
    return true;
}

void extract_reg(char (&com)[100], char (&r)[10], int & j){
    memset(r,0,sizeof(r));
    int len = 0;
    while (com[j] != '$') ++ j;
    ++ j;
    while (j < strlen(com)){
        if ((com[j] <= 'z' && com[j] >= 'a') || (com[j] >= '0' && com[j] <='9'))
            r[len++] = com[j];
        else break;
        ++ j;
    }
    r[len] = '\0';
}

int extract_num(char (&com)[100], int &i){
    int num = 0;
    bool minus = 0;
    while (com[i] > '9' || com[i] < '0'){
        if (com[i] == '-') minus = 1;
        ++ i;
    }
    while (com[i] >= '0' && com[i] <= '9'){
        num = num * 10 + com[i] - '0';
        ++ i;
    }
    if (minus) num = 0 - num;
    return num;
}

/***********
 for test
 ***********/
/*void test(){
    cout << "instruction : " << endl;
    cout << "---------------------" << endl;
    for (int i = 0; i < ins.size(); ++ i){
        cout << ins[i].id << ":" << endl;
        for (int j = 0; j < 3; ++ j) cout << ins[i].par[j] << ' ';
        cout << endl;
    }
}
int Count = 0;*/
ofstream err("error.out");


#endif /* Header_h */
