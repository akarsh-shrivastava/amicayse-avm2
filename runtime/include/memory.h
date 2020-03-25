#ifndef MEMORY_H_DEFINED
    #define MEMORY_H_DEFINED
    #include "avm.h"

    extern wordsized *CS;
    extern wordsized *DS;
    extern wordsized *SS;

    extern wordsized IP;
    extern wordsized SP;
    extern wordsized FLAGS;
    void init_memory();
#endif
