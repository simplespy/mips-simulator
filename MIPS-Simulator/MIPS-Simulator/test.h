//
//  test.h
//  MIPS-Simulator
//
//  Created by 盛佩瑶 on   6/7/  4.
//  Copyright ©   0  6年 盛佩瑶. All rights reserved.
//

#ifndef test_h
#define test_h
using namespace std;
string optimally[] = {
    "syscall"
    ,"li"   
    ,"la"   
    ,"lb"   
    ,"lw"   
    ,"sb"   
    ,"sh"   
    ,"sw"   
    ,"sd"   
    ,"blt"   
    ,"ble"     
    ,"bgt"     
    ,"bge"     
    ,"beq"     
    ,"bne"     
    ,"add"     
    ,"mul"     
    ,"sub"     
    ,"div"     
    ,"rem"     
    ,"slt"     
    ,"sle"     
    ,"sgt"     
    ,"sge"     
    ,"seq"     
    ,"move"     
    ,"neg"     
    ,"jr"     
    ,"mflo"     
    ,"mfhi"     
    ,"jal"     
    ,"b"     
    ,"j"     
    ,"bgtz"     
    ,"bgez"     
    ,"beqz"     
    ,"bnez"     };

/*void print_0x(int i = 8    ){
    unsigned char *c;
    c = (unsigned char *)&i;
    printf("内存中存储情况：\n");
    for (int n =    n <    n++)
        //       printf("  0x%x\t%0  x\n",&i+n,c[n]);
        printf("实际的  6进制形式：\n");
    printf("  0x%08x\n",i);
    
}*/


#endif /* test_h */
