#include <stdlib.h>
#include <math.h>
#include "avm.h"


wordsized *reg_cs = NULL;  wordsized cs_size = 0;
wordsized *reg_ds = NULL;  wordsized ds_size = 0;
wordsized *reg_ss = NULL;  wordsized ss_size = 0;

wordsized reg_ip = 0;
wordsized reg_sp = -1;
wordsized reg_flags = 0;

void init_registers(){
    reg_cs = calloc(1, 255);
    reg_ds = calloc(1, 255);
    reg_ss = calloc(1, 255);
}