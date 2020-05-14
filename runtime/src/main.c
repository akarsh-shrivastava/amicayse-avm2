#include <stdio.h>
#include "memory.h"
#include "instructions.h"
#include "execute.h"

#define NULLU 0

FILE *logfile;

void sample_prog(){
    *(seg_reg[REG_CS]+0) = INST_MOV_ML;
    *(seg_reg[REG_CS]+1) = REG_DS;
    *(seg_reg[REG_CS]+2) = 0;
    *(seg_reg[REG_CS]+3) = NULLU;
    *(seg_reg[REG_CS]+4) = 'H';
    *(seg_reg[REG_CS]+5) = NULLU;
    *(seg_reg[REG_CS]+6) = NULLU;

    *(seg_reg[REG_CS]+7) = INST_MOV_ML;
    *(seg_reg[REG_CS]+8) = REG_DS;
    *(seg_reg[REG_CS]+9) = 1;
    *(seg_reg[REG_CS]+10) = NULLU;
    *(seg_reg[REG_CS]+11) = 'o';
    *(seg_reg[REG_CS]+12) = NULLU;
    *(seg_reg[REG_CS]+13) = NULLU;

    *(seg_reg[REG_CS]+14) = INST_MOV_ML;
    *(seg_reg[REG_CS]+15) = REG_DS;
    *(seg_reg[REG_CS]+16) = 2;
    *(seg_reg[REG_CS]+17) = NULLU;
    *(seg_reg[REG_CS]+18) = 'l';
    *(seg_reg[REG_CS]+19) = NULLU;
    *(seg_reg[REG_CS]+20) = NULLU;

    *(seg_reg[REG_CS]+21) = INST_MOV_ML;
    *(seg_reg[REG_CS]+22) = REG_DS;
    *(seg_reg[REG_CS]+23) = 3;
    *(seg_reg[REG_CS]+24) = NULLU;
    *(seg_reg[REG_CS]+25) = 'a';
    *(seg_reg[REG_CS]+26) = NULLU;
    *(seg_reg[REG_CS]+27) = NULLU;

    *(seg_reg[REG_CS]+28) = INST_MOV_ML;
    *(seg_reg[REG_CS]+29) = REG_DS;
    *(seg_reg[REG_CS]+30) = 4;
    *(seg_reg[REG_CS]+31) = NULLU;
    *(seg_reg[REG_CS]+32) = '\n';
    *(seg_reg[REG_CS]+33) = NULLU;
    *(seg_reg[REG_CS]+34) = NULLU;

    *(seg_reg[REG_CS]+35) = INST_RPUT;
    *(seg_reg[REG_CS]+36) = REG_DS;
    *(seg_reg[REG_CS]+37) = 0;
    *(seg_reg[REG_CS]+38) = NULLU;
    *(seg_reg[REG_CS]+39) = 5;
    *(seg_reg[REG_CS]+40) = NULLU;
    *(seg_reg[REG_CS]+41) = NULLU;

    *(seg_reg[REG_CS]+42) = INST_EXIT;    
}
void main(int argc, const char **argv){
    init_registers();
    init_inst_set_ptr();
    sample_prog();
    logfile = fopen("logs", "w");
    execute(LOG_IP | LOG_DS | LOG_SS | LOG_CS);
    fclose(logfile);
}