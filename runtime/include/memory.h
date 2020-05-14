#ifndef MEMORY_H_DEFINED
    #define MEMORY_H_DEFINED
    #include "avm.h"
    
    enum SegRegNames{
        REG_CS,
        REG_DS,
        REG_SS
    };
    extern wordsized *seg_reg[3]; 
    extern wordsized cs_size;
    extern wordsized ds_size;
    extern wordsized ss_size;

    extern wordsized reg_ip;
    extern wordsized reg_sp;
    extern wordsized reg_flags;

    void init_registers();
#endif
