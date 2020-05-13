#ifndef MEMORY_H_DEFINED
    #define MEMORY_H_DEFINED
    #include "avm.h"

    extern wordsized *reg_cs; extern wordsized cs_size;
    extern wordsized *reg_ds; extern wordsized ds_size;
    extern wordsized *reg_ss; extern wordsized ss_size;

    extern wordsized reg_ip;
    extern wordsized reg_sp;
    extern wordsized reg_flags;
#endif
