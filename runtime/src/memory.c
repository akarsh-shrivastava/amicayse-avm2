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
    seg_reg[REG_CS] = calloc( sizeof(wordsized), pow(2, sizeof(wordsized)*8)-1 );
    seg_reg[REG_DS] = calloc( sizeof(wordsized), pow(2, sizeof(wordsized)*8)-1 );
    seg_reg[REG_SS] = calloc( sizeof(wordsized), pow(2, sizeof(wordsized)*8)-1 );
}