#include <stdlib.h>
#include "avm.h"

wordsized *reg_cs = NULL;
wordsized *reg_ds = NULL;
wordsized *reg_ss = NULL;

wordsized reg_ip = 0;
wordsized reg_sp = -1;
wordsized reg_flags = 0;

void init_memory(){
    reg_cs = calloc(1, 255);
    reg_ds = calloc(1, 255);
    reg_ss = calloc(1, 255);
}