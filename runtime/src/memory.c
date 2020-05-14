#include <stdlib.h>
#include <math.h>
#include "avm.h"
#include "memory.h"


wordsized *seg_reg[3] = {NULL, NULL, NULL};  
wordsized cs_size = 0;
wordsized ds_size = 0;
wordsized ss_size = 0;

wordsized reg_ip = 0;
wordsized reg_sp = -1;
wordsized reg_flags = 0;

void init_registers(){
    seg_reg[REG_CS] = calloc(1, 255);
    seg_reg[REG_DS] = calloc(1, 255);
    seg_reg[REG_SS] = calloc(1, 255);
}