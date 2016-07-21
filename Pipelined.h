#ifndef Pipelined_h
#define Pipelined_h
using namespace std;
const int maxm = 100000000;
int cl = 0;
int state[maxm] = {0};
int exe[maxm];
int head = 0, tail = 0;

extern vector<instruction> ins;
extern long long sum;
bool mem_locked = 0;
bool reg_locked[32] = {0};
bool jump_locked = 0;
bool lo_locked = 0, hi_locked = 0;
/*****state******
 0 : not in the line
 1 : Instruction Fetch
 2 : Instruction Decode & Data Preparation
 3 : Execution
 4 : Memory Access
 5 : Write Back
 ****************/

bool judge(int ind){
    int s = state[ind];
    int idd = exe[ind] > 0 ? ins[exe[ind]].id : 0;
    switch (s) {
        case 0:
            if (mem_locked || jump_locked) return false;
            return true;
        case 1:
            if (exe[ind] < 0){//syscall
                if (reg_locked[2] || reg_locked[4] || reg_locked[5]) return false;
                jump_locked = true; reg_locked[2] = true;
                return true;
            }
            else if (idd == 1){//li
                reg_locked[ins[exe[ind]].par[0]] = true;
                return true;
            }
            else if (idd <= 4) {//la,lb,lw
                if (ins[exe[ind]].par[1] == 2 && reg_locked[ins[exe[ind]].rid[1]]) return false;
                reg_locked[ins[exe[ind]].par[0]] = true;
                return true;
            }
            else if (idd >= 5 && idd <= 8){//sw/sd/sb/sh
                if (reg_locked[ins[exe[ind]].par[0]]) return false;
                if (ins[exe[ind]].par[1] == 2)
                if (reg_locked[ins[exe[ind]].rid[1]]) return false;
                return true;
            }
            else if (idd >= 9 && idd <= 14){//bge/ble/beq/bgt/bne/blt
                if (reg_locked[ins[exe[ind]].par[0]]) return false;
                if (ins[exe[ind]].par[1] == 2 && reg_locked[ins[exe[ind]].rid[1]]) return false;
                jump_locked = true;
                return true;
            }
            else if ((idd >= 15 && idd <= 26) || idd ==37 || idd == 38){//add/mul/sub/div/slt/sle/sgt/sge/seq/rem/move/neg
                if (ins[exe[ind]].offset == -1){//mul-div-2
                    if (reg_locked[ins[exe[ind]].par[1]] || reg_locked[ins[exe[ind]].par[0]]) return false;
                    lo_locked = true; hi_locked = true;
                    return true;
                }
                int reg3 = -1;
                if (ins[exe[ind]].par[2] == 2) reg3 = ins[exe[ind]].rid[2];
                if (reg_locked[ins[exe[ind]].par[1]] || (reg3 != -1 && reg_locked[reg3]))
                    return false;
                reg_locked[ins[exe[ind]].par[0]] = true;
                return true;
            }
            else if (idd == 27) {//jr
                if (reg_locked[ins[exe[ind]].par[0]]) return false;
                jump_locked = true;
                return true;
            }
            else if (idd == 28){//mflo
                if (lo_locked) return false;
                reg_locked[ins[exe[ind]].par[0]] = true;
                return true;
            }
            else if (idd == 29){//mfhi
                if (hi_locked) return false;
                reg_locked[ins[exe[ind]].par[0]] = true;
                return true;
            }
            else if (type[idd] == 7){
                if (idd == 30) reg_locked[31] = true;
                jump_locked = true;
                return true;
            }
            else if (type[idd] == 8){
                if (reg_locked[ins[exe[ind]].par[0]]) return false;
                jump_locked = true;
                return true;
            }
        case 3:
            if (exe[ind] == -4 || exe[ind] == -8 || exe[ind] == -9){
                if (mem_locked) return false;
            }
            if (idd >= 3 && idd <= 8) mem_locked = true;
            return true;
        case 4:
            if (exe[ind] < 0){ jump_locked = false; reg_locked[2] = false;}
            else if (idd == 1) reg_locked[ins[exe[ind]].par[0]] = false;
            else if (idd <= 4) reg_locked[ins[exe[ind]].par[0]] = false;
            else if (idd >= 9 && idd <= 14) jump_locked = false;
            else if ((idd >= 15 && idd <= 26) || idd == 37 || idd == 38){//add/mul/sub/div/slt/sle/sgt/sge/seq/rem/move/neg
                if (ins[exe[ind]].offset == -1){//mul-div-2
                    lo_locked = false; hi_locked = false;
                }
                else reg_locked[ins[exe[ind]].par[0]] = false;
            }
            else if (idd == 27 || type[idd] == 7 || type[idd] == 8){
                jump_locked = false;
                if (idd == 30) reg_locked[31] = false;
            }
            else if (idd == 28 || idd == 29) reg_locked[ins[exe[ind]].par[0]] = false;
            return true;
        default:
            return true;
    }
    return true;
}

void Pipelined(){
    while(head < sum){
        ++ cl;
        mem_locked = false;
        for (int j = head; j <= tail; ++ j){
            if (j == tail && tail < sum-1){
                if (judge(tail)){
                    ++ tail;
                    state[j] = 1;
                    break;
                }
            }
            else if (judge(j)){
                ++ state[j];
                if (state[j] == 6) {
                    ++ head;
                }
            }
            else break;
        }
    }
}
#endif /* Pipelined_h */
