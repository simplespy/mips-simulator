//
//  extract.h
//  MIPS-Simulator
//
//  Created by 盛佩瑶 on 16/7/14.
//  Copyright © 2016年 盛佩瑶. All rights reserved.
//

#ifndef extract_h
#define extract_h
using namespace std;
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


#endif /* extract_h */
