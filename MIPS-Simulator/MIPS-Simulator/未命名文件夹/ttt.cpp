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

int Count = 0;

vector<char*> command;
map<string, unsigned long> lable;
map<unsigned long, unsigned char> memory;
unsigned long pc;

struct Register{
    unsigned long regist[32];
    map<string, unsigned long> Reg;
    Register(){
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
        Reg["t9"] = Reg["26"] = 26;
        Reg["sp"] = Reg["29"] = 29;
        Reg["fp"] = Reg["30"] = 30;
        Reg["ra"] = Reg["31"] = 31;

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
void forge_onward(fstream &fin);
void extract(char *&com, char (&r)[10], int &j);

/***********
   for test
 ***********/
void test(ofstream &out);
void print_0x(int i = 831){
    unsigned char *c;
    c = (unsigned char *)&i;
    printf("内存中存储情况：\n");
    for (int n = 0; n < 4; n++)
 //       printf("  0x%x\t%02x\n",&i+n,c[n]);
    printf("实际的16进制形式：\n");
    printf("  0x%08x\n",i);

}
char* output = new char[100],*input = new char[100];

int main (int argc, const char* argv[]){
    freopen(argv[argc-1],"r",stdin);
    int i = 0;
    for (; i < strlen(argv[argc-1])-1; ++ i){
        input[i] = argv[argc-1][i];
        output[i] = argv[argc-1][i];
    }
    input[i] = 'i';
    output[i++] = 'o';
    input[i] = 'n';
    output[i++] = 'u';
    output[i++] = 't';
    freopen(output,"w",stdout);
    fstream fin(input);
    Read_file();
    ofstream com("command.out");

    test(com);
    ofstream err("error2.out");
  /*  pc = lable["main"];
    while (pc < command.size()) {
        err << command[pc] << endl;
        err << "-------------------" << endl;
        forge_onward(fin);
    }*/
    cerr << Count << endl;
    fin.close();
    err.close();
    com.close();
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
                ++ Count;
                cout << in << endl;
                in[i] = '\0';
                lable[in] = command.size();
                flag = 2;
                break;
            }
            if (in[0] == '\n') {
                flag = 1;
                break;
            }
        }
        if (flag == 0)command.push_back(in);

    }
}


void test(ofstream &out){
    out << "command : " << endl;
    out << "---------------------" << endl;
    for (int i = 0; i < command.size(); ++ i){
        out << command[i] << endl;
    }
}

unsigned long Data_dec(int return_or_not){
    while (!cin.eof()){
        char *in = new char[1024];
        cin.getline(in, 1024);
        for (int i = 0; i < strlen(in); ++ i){
            if (in[i] == ':'){
                ++ Count;
                cout << in << endl;
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
                        memory[memory.size()] = c[n];
                    if (return_or_not == 1) return (memory.size() - 4);
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
                        memory[memory.size()] = 0;
                    }
                    break;
                }
            }
        }
    }
    return 0;
}

void extract(char *&com, char (&r)[10], int &j){
    int len = 0;
    while (j < strlen(com)){
        if ((com[j] <= 'z' && com[j] >= 'a') || (com[j] >= '0' && com[j] <='9'))
            r[len++] = com[j];
        else break;
        ++ j;
    }
    r[len] = '\0';
}


void forge_onward(fstream &fin){
    unsigned long i = pc;
    bool check = 0;
    for (int j = 0; j < strlen(command[i]); ++ j){
        if (command[i][j] == '#') {check = 1; break;}
        //li(Rdest, imm)
        if (command[i][j] == 'l' && command[i][j+1] == 'i'){
            while (command[i][j] != '$') ++ j;
            char r[10];
            extract(command[i], r, ++j);
            
            int num = 0;
            while (command[i][j] < '0' || command[i][j] > '9') ++ j;
            while (command[i][j] >= '0' && command[i][j] <= '9'){
                num = num*10+command[i][j]-'0';
                ++ j;
            }
            reg[r] = num;
            check = 1;
            break;
        }
        //la(Rdest, add)
        else if (command[i][j] == 'l' && command[i][j+1] == 'a'){
            while (command[i][j] != '$') ++ j;
            char r[10];
            extract(command[i], r, ++j);
            
            //lable or offset
            while (command[i][j] == ' ' || command[i][j] == ',') ++ j;
            string lab = "";
            while (j < strlen(command[i])){
                if (command[i][j] == '#' || command[i][j] == ' ') break;
                lab += command[i][j];
                ++ j;
            }
            reg[r] = lable[lab];
            check = 1;
            break;
        }
        //syscall
        else if (command[i][j] == 's' && command[i][j+1] == 'y'){
            switch (reg[2]) {
                case 1:
                    cout << reg[4];// << endl;
                    break;
                case 4:{
                    unsigned long j = reg[4];
                    unsigned char ch = memory[j];
                    //cout << reg[4] << endl;
                    while (ch != '\0') {
                        cout << ch;
                        ch = memory[++j];
                    }
                 //   cout << endl;
                    break;
                }
                case 5:{
                    //cout << "case5" << endl;
                    fin >> reg[2];
                    char ch;
                    ch = fin.get();
                    break;
                }
                case 8:{
                  //  cout << "case8" << endl;
                    char * in = new char[1024];
                    int j = 0;
                    fin.getline(in, reg[5]);
                    
                    while (j < strlen(in)){
                        if (in[j] == '\\' && in[j+1] == 'n'){
                            memory[reg[4]+j] = '\n';
                            j += 2;
                        }
                        else{
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
                    reg[2] = memory.size() - reg[4];
                    break;
                case 10:
                    exit(0);
                case 17:
                    exit(reg[4]);
            }
            check = 1;
            break;
        }
        //sw/sh/sb/sd(Rsrc, add)
        else if (command[i][j] == 's' && command[i][j+1] != 'u'){
            int flag = j+1;
            while (command[i][j] != '$') ++ j;
            char r[10];
            int len = 0;
            while (command[i][++j] != ',' && command[i][j] != ' '){
                r[len++] = command[i][j];
            }
            r[len] = '\0';
            
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
            bool if_reg = false;
            while (command[i][j] == ',' || command[i][j] == ' ') ++ j;
            while (j < strlen(command[i])){
                if (command[i][j] == '#' || command[i][j] == ' ') break;
                if (command[i][j] == '(' || command[i][j] == '$') {
                    if (command[i][j] == '$') if_reg = true;
                    ++j;
                    continue;
                }
                else if (command[i][j] == ')') break;
                lab += command[i][j];
                ++ j;
            }
            
            unsigned long add;
            if (!if_reg) add = lable[lab];
            else add = reg[lab] + del;
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
            check = 1;
            break;
        }
        //lw(Rdest, add)
        else if (command[i][j] == 'l' && command[i][j+1] == 'w'){
            while (command[i][j] != '$') ++ j;
            char r[10];
            extract(command[i], r, ++j);
            
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
            bool if_reg = false;
            while (command[i][j] == ',' || command[i][j] == ' ') ++ j;
            while (j < strlen(command[i])){
                if (command[i][j] == '#' || command[i][j] == ' ') break;
                if (command[i][j] == '(' || command[i][j] == '$') {
                    if (command[i][j] == '$') if_reg = true;
                    ++ j;
                    continue;
                }
                else if (command[i][j] == ')') break;
                lab += command[i][j];
                ++ j;
            }
            
            unsigned long add;
            if (!if_reg) add = lable[lab];
            else add = reg[lab] + del;
            unsigned long num = 0;
            for (int n = 3; n >= 0; -- n)
                num = num << 8 | memory[add+n];
            reg[r] = num;
            check = 1;
            break;
        }

        //jr(Rdest)
        else if (command[i][j] == 'j' && command[i][j+1] == 'r'){
            while (command[i][j] != '$') ++ j;
            char r[10];
            extract(command[i], r, ++j);
            
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
                if (command[i][j] == '#' || command[i][j] == ' ') break;
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
                while (command[i][j] != '$') ++ j;
                char rsrc[10];
                extract(command[i], rsrc, ++j);

                ++ j;
                while (command[i][j] != '$') ++ j;
                char src[10];
                extract(command[i], src, ++j);
                ++ j;
            }
            while (command[i][j] == ',' || command[i][j] == ' ') ++ j;
            string lab = "";
            while (j < strlen(command[i])){
                if (command[i][j] == '#' || command[i][j] == ' ') break;
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
        //add/mul/sub/xor(rdest, rsrc, src)
        else if ((command[i][j] == 'a' && command[i][j+1] == 'd') ||
                (command[i][j] == 'm' && command[i][j+1] == 'u')  ||
                (command[i][j] == 's' && command[i][j+1] == 'u')  ||
                 (command[i][j] == 'x' && command[i][j+1] == 'o'))
        {
            char flag = command[i][j];
            
            while (command[i][j] != '$') ++ j;
            char rdest[10];
            extract(command[i], rdest, ++j);
            ++ j;
            while (command[i][j] != '$') ++ j;
            char r1[10],r2[10];
            extract(command[i], r1, ++j);
            ++ j;
            int tmp = j;
            int num = 0;
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            if (command[i][j] == '$'){
                extract(command[i], r2, ++j);
                ++ j;
                num = reg[r2];
            }
            else {
                j = tmp;
                while (command[i][j] < '0' || command[i][j] > '9') ++ j;
                while (command[i][j] >= '0' && command[i][j] <= '9'){
                    num = num*10+command[i][j]-'0';
                    ++ j;
                }
            }
            switch (flag) {
                case 'a':
                    reg[rdest] = reg[r1] + num;
                    break;
                case 'm':
                    reg[rdest] = reg[r1] * num;
                    break;
                case 's':
                    reg[rdest] = reg[r1] - num;
                    break;
                case 'x':
                    reg[rdest] = reg[r1] xor num;
                    break;
            }
            check = 1;
            break;
        }
        //move(rdest, rsrc)
        else if (command[i][j] == 'm' && command[i][j+1] == 'o'){
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char rdest[10];
            extract(command[i], rdest, ++j);
            ++ j;
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char r1[10];
            extract(command[i], r1, ++j);
            ++ j;
            reg[rdest] = reg[r1];
            check = 1;
            break;
        }

        //slt(rdest, rsrc, src)
        else if (command[i][j] == 's' && command[i][j+1] == 'l'){
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char rdest[10];
            extract(command[i], rdest, ++j);
            ++ j;
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            char r1[10],r2[10];
            extract(command[i], r1, ++j);
            ++ j;
            int tmp = j;
            while (j < strlen(command[i]) && command[i][j] != '$') ++ j;
            if (command[i][j] == '$'){
                extract(command[i], r2, ++j);
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
                if (reg[r1] < num) reg[rdest] = 1;
                else reg[rdest] = 0;
            }
            check = 1;
            break;
        }
    }
    if (check == false){
        cerr << "undefined type : " << command[i] << endl;
    }
    ++ pc;
    return;
}
