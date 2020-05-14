#ifndef INSTRUCTION_H_INCLUDED
    #define INSTRUCTION_H_INCLUDED
    #include "avm.h"
    enum InstNames
    {
        INST_EXIT,

        INST_MOV_MM  , INST_MOV_ML  ,
        INST_LOAD_MM , INST_LOAD_ML ,

        INST_MOD_MMM , INST_MOD_MML , INST_MOD_MLL ,
        INST_DIV_MMM , INST_DIV_MML , INST_DIV_MLL ,
        INST_MUL_MMM , INST_MUL_MML , INST_MUL_MLL ,
        INST_ADD_MMM , INST_ADD_MML , INST_ADD_MLL ,
        INST_SUB_MMM , INST_SUB_MML , INST_SUB_MLL ,

        INST_BAND_MMM, INST_BAND_MML, INST_BAND_MLL,
        INST_BOR_MMM , INST_BOR_MML , INST_BOR_MLL ,
        INST_BXOR_MMM, INST_BXOR_MML, INST_BXOR_MLL,
        INST_BSHL_MMM, INST_BSHL_MML, INST_BSHL_MLL,
        INST_BSHR_MMM, INST_BSHR_MML, INST_BSHR_MLL,

        INST_LAND_MMM, INST_LAND_MML, INST_LAND_MLL,
        INST_LOR_MMM , INST_LOR_MML , INST_LOR_MLL ,
        INST_GRT_MMM , INST_GRT_MML , INST_GRT_MLL ,
        INST_GEQ_MMM , INST_GEQ_MML , INST_GEQ_MLL ,
        INST_EQU_MMM , INST_EQU_MML , INST_EQU_MLL ,
        INST_NEQ_MMM , INST_NEQ_MML , INST_NEQ_MLL ,
        INST_LES_MMM , INST_LES_MML , INST_LES_MLL ,
        INST_LEQ_MMM , INST_LEQ_MML , INST_LEQ_MLL ,

        INST_BNOT_MM , INST_BNOT_ML ,
        INST_BNEG_MM , INST_BNEG_ML ,
        INST_LNOT_MM , INST_LNOT_ML ,

        INST_JMP_C   , INST_JMP_U   ,

        INST_RGET    , INST_RPUT    ,

        INST_PUSH_M  , INST_PUSH_L  ,
        INST_POP_M   , INST_POP_L   ,

        INST_LOAD_SP , INST_LOAD_IP ,
        INST_ADD_SP_M, INST_ADD_SP_L,
        INST_SUB_SP_M, INST_SUB_SP_L
    };

    extern void (*inst_set_ptr[80])(wordsized*, wordsized, wordsized*, wordsized, wordsized*, wordsized);

    void init_inst_set_ptr();
#endif