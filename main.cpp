//
//  main.cpp
//  MIPS-Simulator
//
//  Created by 盛佩瑶 on 16/7/6.
//  Copyright © 2016年 盛佩瑶. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <cstring>
#include <map>
#include "Header.h"
using namespace std;

vector<instruction> ins;
vector<char*> command;
map<string, int> label;
map<unsigned long, unsigned char> memory;

int pc, hi, lo;
char output[100], input[100];

Register reg;
Instruction id;

long long sum = 0, cal = 0, jump = 0, load = 0, store = 0;

void File_config(int &argc, const char *&file);
void Read_file();
int Data_dec(int return_or_not);
void Ins_han();
void get_para(char (&com)[100], instruction *&in, int &i);
void forge_onward();

void extract(char (&com)[100], char (&r)[10], int &j);
void extract_label (char (&in)[100], char (&r)[100], int begin);
bool extract_ins(char (&a)[100], char (&b)[100], int & i);
void extract_reg(char (&com)[100], char (&r)[10], int & j);
int extract_num(char (&com)[100], int &i);
fstream fin("data.s");

int main (int argc, const char* argv[]){
    //freopen(argv[argc-1],"r",stdin);
    //freopen("data.s","r",stdin);
   // File_config(argc, argv[argc-1]);
   // freopen(output,"w",stdout);
    
    //freopen("data.out","w",stdout);
    //fstream fin("data.in");
    
    
    Read_file();
    pc = label["main"];
    while (pc < ins.size()) {
        forge_onward();
    }
    fin.close();
    err.close();
}


void File_config(int & argc, const char *&file){
    int i = 0;
    for (; i < strlen(file)-1; ++ i){
        input[i] = file[i];
        output[i] = file[i];
    }
    input[i] = 'i';
    output[i++] = 'o';
    input[i] = 'n';
    output[i++] = 'u';
    output[i++] = 't';
}

void Read_file(){
    while (!fin.eof()){
        char in[100];
        fin.getline(in, 100);
        for (int i = 0; i < strlen(in); ++ i){
            if (in[i] == '.'){
                if (in[i+1] == 'd') Data_dec(0);
                else if (in[i+1] == 't') Ins_han();
                break;
            }
        }
    }
}


void Ins_han(){
    while(!fin.eof()){
        char in[100];
        fin.getline(in, 100);
        char com[100];
        int i = 0;
        for (int k = 0; k < strlen(in); ++ k){
            if (in[k] == '#') in[k] = '\0';
            if (in[k] == '.' && in[k+1] == 'd'){
                Data_dec(0);
                break;
            }
            else if (in[k] == '.' && in[k+1] == 't'){
                in[k] = '\0';
            }
        }
        if (extract_ins(in, com, i) && strlen(com) > 0){
            instruction *newins = new instruction;
            newins->id = id[com];
            if (id[com] == 0 && !(com[0] == 's' && com[1] == 'y'))
                err << "undefined type : " << com << endl;
            get_para(in,newins,i);
            ins.push_back(*newins);
        }
        else if(strlen(com) > 0){
            label[com] = (int)ins.size();
        }
    }
}

void get_para(char (&com)[100], instruction *&in, int &i){
    char r[10];
    char lab[100];
    int tmp;
    bool flag, flag2;
    if (type[in->id] == 0) return;//syscall
    if (type[in->id] == 7){//jal/b/j
        extract_label(com, in->la[0], i);
    }
    else {
        while (com[i] != '$') ++ i;
        extract_reg(com, r, i);
        in->par[0] = reg.get_id(r);
        switch (type[in->id]) {
            case 1 ://li(Reg, imm)
                in->par[1] = extract_num(com, i);
                break;
            case 2 ://la/lb/sw/sd/sb/sh/lw(Reg, label/offset)
                tmp = i;
                flag = 0;
                while (tmp < strlen(com)){
                    if (com[tmp] == '(') {flag = 1;break;}
                    ++ tmp;
                }
                if (flag) {//offset
                    tmp = extract_num(com, i);
                    extract_reg(com, r, i);
                    in->par[1] = 2;
                    in->rid[1] = reg.get_id(r);
                    in->offset = tmp;
                }
                else {//label
                    in->par[1] = 1;
                    extract_label(com, in->la[1], i);
                }
                break;
            case 3 ://bge/ble/beq/bgt/bne/blt(Reg, Reg/imm, label)
                tmp = i;
                flag = 0;
                while (tmp < strlen(com)){
                    if (com[tmp] == '$') {flag = 1;break;}
                    ++ tmp;
                }
                if (flag){//Reg
                    extract_reg(com, r, i);
                    in->par[1] = 2;
                    in->rid[1] = reg.get_id(r);
                }
                else {//imm
                    in->par[1] = 3;
                    in->number[1] = extract_num(com, i);
                }
                extract_label(com, in->la[2], i);
                break;
            case 4 ://add/mul/sub/div/slt/sle/sgt/sge/seq/rem(Reg, Reg, Reg/imm/none)
                extract_reg(com, r, i);
                in->par[1] = reg.get_id(r);
                tmp = i;
                flag = flag2 = 0;
                while (tmp < strlen(com)){
                    if (com[tmp] == '$') {flag = flag2 = 1;break;}
                    if ((com[tmp] >= 'a' && com[tmp] <= 'z') || (com[tmp] >= '0' && com[tmp] <= '9')){flag2 = 1;}
                    ++ tmp;
                }
                if (!flag2){
                    in->offset = -1;
                }//none
                else {
                    if (flag){//Reg
                        extract_reg(com, r, i);
                        in->par[2] = 2;
                        in->rid[2] = reg.get_id(r);
                    }
                    else {//imm
                        in->par[2] = 3;
                        in->number[2] = extract_num(com, i);
                    }

                }
                break;
            case 5 ://move(Reg, Reg)
                extract_reg(com, r, i);
                in->par[1] = reg.get_id(r);
                break;
            case 8 ://beqz/bnez/bgez/bgtz(Reg, label)
                extract_label(com, in->la[1], i);
                break;
        }
    }
}


int Data_dec(int return_or_not){
    while (!fin.eof()){
        char in[100];
        fin.getline(in, 100);
        for (int i = 0; i < strlen(in); ++ i){
            if (in[i] == ':'){
                char l[100];
                extract_label(in, l, 0);
                label[l] = Data_dec(1);
            }
            else if (in[i] == '#'){
                in[i] = '\0';
            }
            else if (in[i] == '.'){
                if (in[i+1] == 't'){
                    //text
                    Ins_han();
                }
                //.word
                else if (in[i+1] == 'w'){
                    int num = 0;
                    while (i < strlen(in)){
                        if (in[i] >= '0' && in[i] <= '9')
                            num = num*10+in[i]-'0';
                        ++ i;
                    }
                    unsigned char *c = (unsigned char*)&num;
                    for (int n = 0; n < 4; ++ n)
                        memory[memory.size()] = c[n];
                    if (return_or_not == 1) return int(memory.size() - 4);
                }//.word
               
                //.ascii(z)
                else if (in[i+1] == 'a' && in[i+2] == 's'){
                    bool flag = false;
                    if (in[i+6] == 'z') flag = true;
                    while (in[i] != '"') ++ i;
                    unsigned long len = 0;
                    while (in[i+1] != '"'){
                        if (in[i+1] == '\\'){
                            switch (in[i+2]) {
                                case 'n':
                                    memory[memory.size()] = '\n';
                                    break;
                                case '\\':
                                    memory[memory.size()] = '\\';
                                    break;
                                case '"':
                                    memory[memory.size()] = '\"';
                            }
                            ++ len;
                            i += 2;
                        }
                        else{
                            memory[memory.size()] = in[i+1];
                            ++ len;
                            ++ i;
                        }
                    }
                    if (flag) {
                        memory[memory.size()] = '\0';
                        ++ len;
                        ++ i;
                    }
                    if (return_or_not == 1) return int(memory.size() - len);
                }//.ascii(z)
                //.align
                else if (in[i+1] == 'a' && in[i+2] == 'l'){
                    int num = 0;
                    while (i < strlen(in)){
                        if (in[i] >= '0' && in[i] <= '9')
                            num = num*10+in[i]-'0';
                        ++ i;
                    }
                    unsigned long mod;
                    unsigned long base = 1;
                    for (int j = 0; j < num; ++ j) base = (base << 1);
                    mod = memory.size() % base;
                    if (mod) mod = base - mod;
                    while (mod --){
                        memory[memory.size()] = 0;
                    }
                    break;
                }
            }
        }
    }
    return 0;
}
//syscall
void fun00(){
    switch (reg[2]) {
        case 1:
            cout << reg[4];// << endl;
            break;
        case 4:{
            unsigned long j = reg[4];
            unsigned char ch = memory[j];
            while (ch != '\0') {
                cout << ch;//to be changed
                ch = memory[++j];
            }
            break;
        }
        case 5:{
            cin >> reg[2];
            char ch;
            ch = cin.get();//to be checked
            break;
        }
        case 8:{
            char in[1024];
            int j = 0;
            cin.getline(in, reg[5]);
            while (j < strlen(in)){
                if (in[j] == '\\' && in[j+1] == 'n'){
                    memory[reg[4]+j] = '\n';
                    j += 2;
                }else{
                    memory[reg[4]+j] = in[j];
                    ++ j;
                }
            }
            break;
        }
        case 9:
            unsigned long mod;
            mod = memory.size() % 4;
            if (mod) mod = 4 - mod;
            while (mod --){
                memory[memory.size()] = 0;
            }//align 2^2
            
            for (int i = 0; i < reg[4]; ++ i){
                memory[memory.size()] = 0;
            }
            reg[2] = (int)memory.size() - reg[4];
            break;
        case 10:
            exit(0);
        case 17:
            exit(reg[4]);
    }
    ++ pc;
}
//li(Rdest, imm)
void fun1(int i){
    ++ load;
    reg[ins[i].par[0]] = ins[i].par[1];
    ++ pc;
}
//la(Rdest, label/offset)
void fun2(int i){
    ++ load;
    if (ins[i].par[1] == 1){
        reg[ins[i].par[0]] = label[ins[i].la[1]];
    }
    else {
        reg[ins[i].par[0]] = reg[ins[i].rid[1]] + ins[i].offset;
    }
    ++ pc;
}
//lb(Rdest, label/offset)
void fun3(int i){
    ++ load;
    unsigned long addr = 0;
    if (ins[i].par[1] == 1){
        addr = label[ins[i].la[1]];
    }
    else {
        addr = reg[ins[i].rid[1]] + ins[i].offset;
    }
    int num = 0;
    for (int n = 0; n >= 0; -- n)
        num = num << 8 | memory[addr+n];
    reg[ins[i].par[0]] = num;
    ++ pc;
}
//lw(Rdest, label/offset)
void fun4(int i){
    ++ load;
    unsigned long addr = 0;
    if (ins[i].par[1] == 1){
        addr = label[ins[i].la[1]];
    }
    else {
        addr = reg[ins[i].rid[1]] + ins[i].offset;
    }
    int num = 0;
    for (int n = 3; n >= 0; -- n)
        num = num << 8 | memory[addr+n];
    reg[ins[i].par[0]] = num;
    ++ pc;
}
//sw/sh/sb/sd(Rsrc, add)
void fun5(int i){
    ++ store;
    unsigned long addr = 0;
    if (ins[i].par[1] == 1){
        addr = label[ins[i].la[1]];
    }
    else {
        addr = reg[ins[i].rid[1]] + ins[i].offset;
    }
    unsigned char *c = (unsigned char*)&reg[ins[i].par[0]];
    for (int n = 0; n < 1; ++ n)
        memory[addr+n] = c[n];
    ++ pc;
}
void fun6(int i){
    ++ store;
    unsigned long addr = 0;
    if (ins[i].par[1] == 1){
        addr = label[ins[i].la[1]];
    }
    else {
        addr = reg[ins[i].rid[1]] + ins[i].offset;
    }
    unsigned char *c = (unsigned char*)&reg[ins[i].par[0]];
    for (int n = 0; n < 2; ++ n)
        memory[addr+n] = c[n];
    ++ pc;
}

void fun7(int i){
    ++ store;
    unsigned long addr = 0;
    if (ins[i].par[1] == 1){
        addr = label[ins[i].la[1]];
    }
    else {
        addr = reg[ins[i].rid[1]] + ins[i].offset;
    }
    unsigned char *c = (unsigned char*)&reg[ins[i].par[0]];
    for (int n = 0; n < 4; ++ n)
        memory[addr+n] = c[n];
    ++ pc;
}

void fun8(int i){
    ++ store;
    unsigned long addr = 0;
    if (ins[i].par[1] == 1){
        addr = label[ins[i].la[1]];
    }
    else {
        addr = reg[ins[i].rid[1]] + ins[i].offset;
    }
    unsigned char *c = (unsigned char*)&reg[ins[i].par[0]];
    for (int n = 0; n < 8; ++ n)
        memory[addr+n] = c[n];
    ++ pc;
}
//blt/ble/bgt/bge/beq/bne(Reg, Reg/imm, label)
void fun9(int i){
    int num;
    if (ins[i].par[1] == 2){
        num = reg[ins[i].rid[1]];
    }
    else {
        num = ins[i].number[1];
    }
    if (reg[ins[i].par[0]] < num) pc = label[ins[i].la[2]];
    else ++ pc;
}
void fun10(int i){
    int num;
    if (ins[i].par[1] == 2){
        num = reg[ins[i].rid[1]];
    }
    else {
        num = ins[i].number[1];
    }
    if (reg[ins[i].par[0]] <= num) pc = label[ins[i].la[2]];
    else ++ pc;
}
void fun11(int i){
    int num;
    if (ins[i].par[1] == 2){
        num = reg[ins[i].rid[1]];
    }
    else {
        num = ins[i].number[1];
    }
    if (reg[ins[i].par[0]] > num) pc = label[ins[i].la[2]];
    else ++ pc;
}
void fun12(int i){
    int num;
    if (ins[i].par[1] == 2){
        num = reg[ins[i].rid[1]];
    }
    else {
        num = ins[i].number[1];
    }
    if (reg[ins[i].par[0]] >= num) pc = label[ins[i].la[2]];
    else ++ pc;
}
void fun13(int i){
    int num;
    if (ins[i].par[1] == 2){
        num = reg[ins[i].rid[1]];
    }
    else {
        num = ins[i].number[1];
    }
    if (reg[ins[i].par[0]] == num) pc = label[ins[i].la[2]];
    else ++ pc;
}
void fun14(int i){
    int num;
    if (ins[i].par[1] == 2) num = reg[ins[i].rid[1]];
    else num = ins[i].number[1];
    if (reg[ins[i].par[0]] != num) pc = label[ins[i].la[2]];
    else ++ pc;
}
//add/mul/sub/div(rdest, rsrc, reg/imm/none)
void fun15(int i){
    ++ cal;
    int num;
    if (ins[i].par[2] == 2) num = reg[ins[i].rid[2]];
    else num = ins[i].number[2];
    reg[ins[i].par[0]] = reg[ins[i].par[1]] + num;
    ++ pc;
}
void fun16(int i){
    ++ cal;
    int num;
    if (ins[i].offset == -1){
        long long ans;
        ans = (long long)reg[ins[i].par[0]] * reg[ins[i].par[1]];
        lo = (int)ans;
        hi = ans >> 32;
    }
    else {
        if (ins[i].par[2] == 2) num = reg[ins[i].rid[2]];
        else num = ins[i].number[2];
        reg[ins[i].par[0]] = reg[ins[i].par[1]] * num;
    }
        ++ pc;
}
void fun17(int i){
    ++ cal;
    int num;
    if (ins[i].par[2] == 2) num = reg[ins[i].rid[2]];
    else num = ins[i].number[2];
    reg[ins[i].par[0]] = reg[ins[i].par[1]] - num;
    ++ pc;
}
void fun18(int i){
    ++ cal;
    int num;
    if (ins[i].offset == -1){
        lo = reg[ins[i].par[0]] / reg[ins[i].par[1]];
        hi = reg[ins[i].par[0]] % reg[ins[i].par[1]];
    }
    else {
        if (ins[i].par[2] == 2) num = reg[ins[i].rid[2]];
        else num = ins[i].number[2];
        reg[ins[i].par[0]] = reg[ins[i].par[1]] / num;
    }
    ++ pc;
}
void fun19(int i){
    ++ cal;
    int num;
    if (ins[i].par[2] == 2) num = reg[ins[i].rid[2]];
    else num = ins[i].number[2];
    reg[ins[i].par[0]] = reg[ins[i].par[1]] % num;
    ++ pc;
}
//slt/sle/sgt/sge/seq
void fun20(int i){
    int num;
    if (ins[i].par[2] == 2) num = reg[ins[i].rid[2]];
    else num = ins[i].number[2];
    reg[ins[i].par[0]] = reg[ins[i].par[1]] < num ? 1 : 0;
    ++ pc;
}
void fun21(int i){
    int num;
    if (ins[i].par[2] == 2) num = reg[ins[i].rid[2]];
    else num = ins[i].number[2];
    reg[ins[i].par[0]] = reg[ins[i].par[1]] <= num ? 1 : 0;
    ++ pc;
}void fun22(int i){
    int num;
    if (ins[i].par[2] == 2) num = reg[ins[i].rid[2]];
    else num = ins[i].number[2];
    reg[ins[i].par[0]] = reg[ins[i].par[1]] > num ? 1 : 0;
    ++ pc;
}void fun23(int i){
    int num;
    if (ins[i].par[2] == 2) num = reg[ins[i].rid[2]];
    else num = ins[i].number[2];
    reg[ins[i].par[0]] = reg[ins[i].par[1]] >= num ? 1 : 0;
    ++ pc;
}void fun24(int i){
    int num;
    if (ins[i].par[2] == 2) num = reg[ins[i].rid[2]];
    else num = ins[i].number[2];
    reg[ins[i].par[0]] = reg[ins[i].par[1]] == num ? 1 : 0;
    ++ pc;
}
//move(rdest, rsrc)
void fun25(int i){
    reg[ins[i].par[0]] = reg[ins[i].par[1]];
    ++ pc;
}
//neg(Rdest, Rsrc)
void fun26(int i){
    reg[ins[i].par[0]] = 0 - reg[ins[i].par[1]];
    ++ pc;
}
//jr(Rdest)
void fun27(int i){
    ++ jump;
    pc = reg[ins[i].par[0]];
}
//mflo/mfhi(Rdest)
void fun28(int i){
    reg[ins[i].par[0]] = lo;
    ++ pc;
}
void fun29(int i){
    reg[ins[i].par[0]] = hi;
    ++ pc;
}
//jal/b/j(label)
void fun30(int i){
    ++ jump;
    reg[31] = pc + 1;
    pc = label[ins[i].la[0]];
}
void fun31(int i){
    pc = label[ins[i].la[0]];
}
void fun32(int i){
    ++ jump;
    pc = label[ins[i].la[0]];
}

//bgtz/bgez/beqz/bnez(Reg, label)
void fun33(int i){
    if (reg[ins[i].par[0]] > 0) pc = label[ins[i].la[1]];
    else ++ pc;
}
void fun34(int i){
    if (reg[ins[i].par[0]] >= 0) pc = label[ins[i].la[1]];
    else ++ pc;
}
void fun35(int i){
    if (reg[ins[i].par[0]] == 0) pc = label[ins[i].la[1]];
    else ++ pc;
}
void fun36(int i){
    if (reg[ins[i].par[0]] != 0) pc = label[ins[i].la[1]];
    else ++ pc;
}
//xor
void fun37(int i){
    ++ cal;
    int num;
    if (ins[i].par[2] == 2) num = reg[ins[i].rid[2]];
    else num = ins[i].number[2];
    reg[ins[i].par[0]] = reg[ins[i].par[1]] xor num;
    ++ pc;
}
//sne
void fun38(int i){
    int num;
    if (ins[i].par[2] == 2) num = reg[ins[i].rid[2]];
    else num = ins[i].number[2];
    reg[ins[i].par[0]] = reg[ins[i].par[1]] != num ? 1 : 0;
    ++ pc;
}

void forge_onward(){
    ++ sum;
    //err << optimally[ins[pc].id] << endl;
    if (ins[pc].id == 0) fun00();
    else f[ins[pc].id](pc);
}
