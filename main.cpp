//
//  main.cpp
//  TerSP
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
using namespace std;


vector<char*> command;
map<string, unsigned long> lable;
vector<unsigned char> memory;
unsigned long pc;

struct Register{
    unsigned long regist[32];
    map<string, unsigned long> Reg;
    Register(){
        Reg["v0"] = 2;
        Reg["v1"] = 3;
        Reg["a0"] = 4;
        Reg["a1"] = 5;
        Reg["a2"] = 6;
        Reg["a3"] = 7;
        Reg["t0"] = 8;
        Reg["t1"] = 9;
        Reg["t2"] = 10;
        Reg["t3"] = 11;
        Reg["t4"] = 12;
        Reg["t5"] = 13;
        Reg["t6"] = 14;
        Reg["t7"] = 15;
        Reg["s0"] = 16;
        Reg["s1"] = 17;
        Reg["s2"] = 18;
        Reg["s3"] = 19;
        Reg["s4"] = 20;
        Reg["s5"] = 21;
        Reg["s6"] = 22;
        Reg["s7"] = 23;
        Reg["t8"] = 24;
        Reg["t9"] = 26;
        Reg["sp"] = 29;
        Reg["fp"] = 30;
        Reg["ra"] = 31;

        regist[29] = 0x80000000;
    }
    unsigned long& operator [](string lab){
        if (Reg[lab] == 0) return regist[0] = 0;
        return regist[Reg[lab]];
    }
    unsigned long& operator [](int id){
        if (id == 0) return regist[0] = 0;
        return regist[id];
    }
}reg;


void Read_file();
unsigned long Data_dec(int return_or_not);
void forge_onward();

/***********
   for test
 ***********/
void test();
void print_0x(int i = 831){
    unsigned char *c;
    c = (unsigned char *)&i;
    printf("内存中存储情况：\n");
    for (int n = 0; n < 4; n++)
 //       printf("  0x%x\t%02x\n",&i+n,c[n]);
    printf("实际的16进制形式：\n");
    printf("  0x%08x\n",i);

}


int main (int argc, const char* argv[]){
//    freopen(argv[argc-1],"r",stdin);
//    freopen("argv.out","w",stdout);
    Read_file();
    test();
    pc = lable["main"];
    while (pc < command.size()) {
        forge_onward();
    }
}

void Read_file(){
    while (!cin.eof()){
        char *in = new char[1024];
        cin.getline(in, 1024);
        int flag = 0;
        for (int i = 0; i < strlen(in); ++ i){
            if (in[i] == '.'){
                if (in[i+1] == 'd') Data_dec(0);
                else if (in[i>+1] == 't') break;
                flag = 1;
                break;
            }
            else if (in[i] == ':'){
                in[i] = '\0';
                lable[in] = command.size();
                flag = 2;
                break;
            }
            else if (in[i] == '#') {
                flag = 1;
                break;
            }
        }
        if (flag == 0)command.push_back(in);

    }
}


void test(){
    cout << "command : " << endl;
    cout << "---------------------" << endl;
    for (int i = 0; i < command.size(); ++ i){
        cout << command[i] << endl;
    }
}

unsigned long Data_dec(int return_or_not){
    while (!cin.eof()){
        char *in = new char[1024];
        cin.getline(in, 1024);
        for (int i = 0; i < strlen(in); ++ i){
            if (in[i] == ':'){
                in[i] = '\0';
                lable[in] = Data_dec(1);
            }
            else if (in[i] == '#'){
                break;
            }
            else if (in[i] == '.'){
                if (in[i+1] == 't'){
                    //text
                    return 0;
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
                        memory.push_back(c[n]);
                    if (return_or_not == 1) return (memory.size() - 4);
                }//.word
               
                //.ascii(z)
                else if (in[i+1] == 'a' && in[i+2] == 's'){
                    bool flag = false;
                    if (in[i+6] == 'z') flag = true;
                    while (in[i] != '"') ++ i;
                    unsigned long len = 0;
                    while (in[i+1] != '"'){
                        if (in[i+1] == '\\' && in[i+2] == 'n'){
                            memory.push_back('\n');
                            ++ len;
                            i += 2;
                        }
                        else{
                            memory.push_back(in[i+1]);
                            ++ len;
                            ++ i;
                        }
                    }
                    if (flag) {
                        memory.push_back('\0');
                        ++ len;
                        ++ i;
                    }
                    if (return_or_not == 1) return (memory.size() - len);
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
                        memory.push_back(0);
                    }
                    break;
                }
            }
        }
    }
    return 0;
}

void forge_onward(){
    unsigned long i = pc;
    for (int j = 0; j < strlen(command[i]); ++ j){
        //li(Rdest, imm)
        if (command[i][j] == 'l' && command[i][j+1]){
            while (j < strlen(command[i]) && command[i][j] != '$'){
                ++ j;
            }
            char r[3];
            r[0] = command[i][++j];
            r[1] = command[i][++j];
            r[2] = '\0';
            ++ j;
            int num = 0;
            while (command[i][j] < '0' || command[i][j] > '9') ++ j;
            while (command[i][j] >= '0' && command[i][j] <= '9'){
                num = num*10+command[i][j]-'0';
                ++ j;
            }
            reg[r] = num;
        }
        //la(Rdest, add)
        else if (command[i][j] == 'l' && command[i][j+1] == 'a'){
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char r[3];
            r[0] = command[i][++j];
            r[1] = command[i][++j];
            r[2] = '\0';
            ++ j;
            //lable or offset
            while (command[i][j] == ' ' || command[i][j] == ',') ++ j;
            string lab = "";
            while (j < strlen(command[i])){
                lab += command[i][j];
                ++ j;
            }
            reg[r] = lable[lab];
        }
        //syscall
        else if (command[i][j] == 's' && command[i][j+1] == 'y'){
            switch (reg[2]) {
                case 1:
                    cout << reg[4] << endl;
                    break;
                case 4:{
                    char *ch = reinterpret_cast<char*>(reg[4]);
                    cout << ch << endl;
                    break;
                }
                case 5:
                    cin >> reg[2];
                    break;
                case 8:{
                    char * in = reinterpret_cast<char*>(reg[4]);
                    cin.getline(in,reg[5]);
                    break;
                }
                case 9:
                    for (int i = 0; i < reg[4]; ++ i){
                        memory.push_back(0);
                    }
                    reg[2] = memory.size() - reg[4];
                    break;
                case 10:
                    return;
                    break;
                case 17:
                    return;
                    break;
            }
        }
        //sw/sh/sb/sd(Rsrc, add)
        else if (command[i][j] == 's'){
            int flag = j+1;
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char r[3];
            r[0] = command[i][++j];
            r[1] = command[i][++j];
            r[2] = '\0';
            ++ j;
            while (command[i][j] == ',' || command[i][j] == ' ') ++ j;
            bool minus = 0;
            int del = 0;
            if (command[i][j] == '-') {
                minus = true;
                ++ j;
            }
            while (command[i][j] >= '0' && command[i][j] <= '9'){
                del = del * 10 + command[i][j] - '0';
                ++ j;
            }
            if (minus) del = 0 - del;
            string lab = "";
            ++ j;
            while (j < strlen(command[i])){
                if (command[i][j] == '(') continue;
                else if (command[i][j] == ')') break;
                lab += command[i][j];
                ++ j;
            }
            
            unsigned long add = lable[lab]+del;
            unsigned long num = reg[r];
            unsigned char *c = (unsigned char*)&num;
            int to = 0;
            switch (command[i][flag]) {
                case 'w':
                    to = 4;
                    break;
                case 'h':
                    to = 2;
                    break;
                case 'd':
                    to = 8;
                    break;
                case 'b':
                    to = 1;
                    break;
            }
            for (int n = 0; n < to; ++ n)
                memory[add+n] = c[n];
        }
        //lw(Rdest, add)
        else if (command[i][j] == 'l' && command[i][j] == 'w'){
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char r[3];
            r[0] = command[i][++j];
            r[1] = command[i][++j];
            r[2] = '\0';
            ++ j;
            while (command[i][j] == ',' || command[i][j] == ' ') ++ j;
            bool minus = 0;
            int del = 0;
            if (command[i][j] == '-') {
                minus = true;
                ++ j;
            }
            while (command[i][j] >= '0' && command[i][j] <= '9'){
                del = del * 10 + command[i][j] - '0';
                ++ j;
            }
            if (minus) del = 0 - del;
            string lab = "";
            ++ j;
            while (j < strlen(command[i])){
                if (command[i][j] == '(') continue;
                else if (command[i][j] == ')') break;
                lab += command[i][j];
                ++ j;
            }
            
            unsigned long add = lable[lab]+del;
            unsigned long num = 0;
            for (int n = 3; n >= 0; -- n)
                num = num << 8 | memory[add+n];
            reg[r] = num;
        }

        //jr
        else if (command[i][j] == 'j' && command[i][j] == 'r'){
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char r[3];
            r[0] = command[i][++j];
            r[1] = command[i][++j];
            r[2] = '\0';
            pc = reg[r];
            return;
        }
        //jal(label)-jump and link
        else if (command[i][j] == 'j' && (command[i][j+1] == 'a' || command[i][j+1] == ' ')){
            int flag = j+1;
            j += 3;
            while (command[i][j] == ' ') ++ j;
            string lab = "";
            while (j < strlen(command[i])){
                lab += command[i][j];
                ++ j;
            }
            if (command[i][flag] == 'a') reg[31] = pc+1;
            pc = lable[lab];
            return;
        }
        //b/bge/beq/ble(Rsrc, src, label)
        else if (command[i][j] == 'b'){
            int flag = ++j;
            char rsrc[3];
            char src[3];
            if (command[i][flag] != ' '){
                while (j < strlen(command[i]) && command[i][j] != '$')++ j;
                rsrc[0] = command[i][++j];
                rsrc[1] = command[i][++j];
                rsrc[2] = '\0';
                ++ j;
                while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
                src[0] = command[i][++j];
                src[1] = command[i][++j];
                src[2] = '\0';
                ++ j;
            }
            while (command[i][j] == ',' || command[i][j] == ' ') ++ j;
            string lab = "";
            while (j < strlen(command[i])){
                lab += command[i][j];
                ++ j;
            }
            switch (command[i][flag]) {
                case 'e':
                    if (reg[rsrc] == reg[src]) pc = lable[lab];
                    break;
                case 'g':
                    if (reg[rsrc] >= reg[src]) pc = lable[lab];
                    break;
                case 'l':
                    if (reg[rsrc] <= reg[src]) pc = lable[lab];
                    break;
                case ' ':
                    pc = lable[lab];
            }
            return;
        }
        //add(rdest, rsrc, src)
        else if (command[i][j] == 'a' && command[i][j+1] == 'd'){
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char rdest[3];
            rdest[0] = command[i][++j];
            rdest[1] = command[i][++j];
            rdest[2] = '\0';
            ++ j;
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char r1[3],r2[3];
            r1[0] = command[i][++j];
            r1[1] = command[i][++j];
            r1[2] = '\0';
            ++ j;
            int tmp = j;
            while (j < strlen(command[i]) && command[i][j] != '$'){
                ++ j;
            }
            if (command[i][j] == '$'){
                r2[0] = command[i][++j];
                r2[1] = command[i][++j];
                r2[2] = '\0';
                ++ j;
                reg[rdest] = reg[r1] + reg[r2];
            }
            else {
                j = tmp;
                int num = 0;
                while (command[i][j] < '0' || command[i][j] > '9') ++ j;
                while (command[i][j] >= '0' && command[i][j] <= '9'){
                    num = num*10+command[i][j]-'0';
                    ++ j;
                }
                reg[rdest] = reg[r1] + num;
            }
        }
        //sub
        else if (command[i][j] == 's' && command[i][j+1] == 'u'){
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char rdest[3];
            rdest[0] = command[i][++j];
            rdest[1] = command[i][++j];
            rdest[2] = '\0';
            ++ j;
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char r1[3],r2[3];
            r1[0] = command[i][++j];
            r1[1] = command[i][++j];
            r1[2] = '\0';
            ++ j;
            int tmp = j;
            while (j < strlen(command[i]) && command[i][j] != '$'){
                ++ j;
            }
            if (command[i][j] == '$'){
                r2[0] = command[i][++j];
                r2[1] = command[i][++j];
                r2[2] = '\0';
                ++ j;
                reg[rdest] = reg[r1] - reg[r2];
            }
            else {
                j = tmp;
                int num = 0;
                while (command[i][j] < '0' || command[i][j] > '9') ++ j;
                while (command[i][j] >= '0' && command[i][j] <= '9'){
                    num = num*10+command[i][j]-'0';
                    ++ j;
                }
                reg[rdest] = reg[r1] - num;
            }
        }

        //mul
        else if (command[i][j] == 'm' && command[i][j+1] == 'u'){
            while (j < strlen(command[i]) && command[i][j] != '$'){
                ++ j;
            }
            char rdest[3];
            rdest[0] = command[i][++j];
            rdest[1] = command[i][++j];
            rdest[2] = '\0';
            ++ j;
            while (j < strlen(command[i]) && command[i][j] != '$'){
                ++ j;
            }
            char r1[3],r2[3];
            r1[0] = command[i][++j];
            r1[1] = command[i][++j];
            r1[2] = '\0';
            ++ j;
            int tmp = j;
            while (j < strlen(command[i]) && command[i][j] != '$'){
                ++ j;
            }
            if (command[i][j] == '$'){
                r2[0] = command[i][++j];
                r2[1] = command[i][++j];
                r2[2] = '\0';
                ++ j;
                reg[rdest] = reg[r1] * reg[r2];
            }
            else {
                j = tmp;
                int num = 0;
                while (command[i][j] < '0' || command[i][j] > '9') ++ j;
                while (command[i][j] >= '0' && command[i][j] <= '9'){
                    num = num*10+command[i][j]-'0';
                    ++ j;
                }
                reg[rdest] = reg[r1] * num;
            }
        }
        //add(rdest, rsrc, src)
        else if (command[i][j] == 'a' && command[i][j+1] == 'd'){
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char rdest[3];
            rdest[0] = command[i][++j];
            rdest[1] = command[i][++j];
            rdest[2] = '\0';
            ++ j;
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char r1[3],r2[3];
            r1[0] = command[i][++j];
            r1[1] = command[i][++j];
            r1[2] = '\0';
            ++ j;
            int tmp = j;
            while (j < strlen(command[i]) && command[i][j] != '$'){
                ++ j;
            }
            if (command[i][j] == '$'){
                r2[0] = command[i][++j];
                r2[1] = command[i][++j];
                r2[2] = '\0';
                ++ j;
                reg[rdest] = reg[r1] + reg[r2];
            }
            else {
                j = tmp;
                int num = 0;
                while (command[i][j] < '0' || command[i][j] > '9') ++ j;
                while (command[i][j] >= '0' && command[i][j] <= '9'){
                    num = num*10+command[i][j]-'0';
                    ++ j;
                }
                reg[rdest] = reg[r1] + num;
            }
        }
        //move(rdest, rsrc)
        else if (command[i][j] == 'm' && command[i][j+1] == 'o'){
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char rdest[3];
            rdest[0] = command[i][++j];
            rdest[1] = command[i][++j];
            rdest[2] = '\0';
            ++ j;
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char r1[3];
            r1[0] = command[i][++j];
            r1[1] = command[i][++j];
            r1[2] = '\0';
            ++ j;
            reg[rdest] = reg[r1];
        }

        //slt(rdest, rsrc, src)
        else if (command[i][j] == 's' && command[i][j+1] == 'l'){
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char rdest[3];
            rdest[0] = command[i][++j];
            rdest[1] = command[i][++j];
            rdest[2] = '\0';
            ++ j;
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char r1[3],r2[3];
            r1[0] = command[i][++j];
            r1[1] = command[i][++j];
            r1[2] = '\0';
            ++ j;
            int tmp = j;
            while (j < strlen(command[i]) && command[i][j] != '$'){
                ++ j;
            }
            if (command[i][j] == '$'){
                r2[0] = command[i][++j];
                r2[1] = command[i][++j];
                r2[2] = '\0';
                ++ j;
                if (reg[r1] < reg[r2]) reg[rdest] = 1;
                else reg[rdest] = 0;
            }
            else {
                j = tmp;
                int num = 0;
                while (command[i][j] < '0' || command[i][j] > '9') ++ j;
                while (command[i][j] >= '0' && command[i][j] <= '9'){
                    num = num*10+command[i][j]-'0';
                    ++ j;
                }
                reg[rdest] = reg[r1] + num;
            }
        }


    }
    ++ pc;
    return;
}
