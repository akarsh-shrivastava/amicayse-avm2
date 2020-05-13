#ifdef __linux__
    #include <unistd.h>
#else
    #error Only Linux supported
#endif

#include "instructions.h"
#include "memory.h"






// arithmetic, logical and binary operators
#define MMM_INST(name, operator) \
void name##_mmm(wordsized* seg0, wordsized dest, wordsized* seg1, wordsized op1, wordsized* seg2, wordsized op2){\
    *(seg0+dest) = *(seg1+op1) operator *(seg2+op2);\
}
#define MML_INST(name, operator) \
void name##_mml(wordsized* seg0, wordsized dest, wordsized* seg1, wordsized op1, wordsized* dummy1, wordsized op2){\
    *(seg0+dest) = *(seg1+op1) operator op2;\
}
#define MLL_INST(name, operator) \
void name##_mll(wordsized* seg0, wordsized dest, wordsized* dummy0, wordsized op1, wordsized* dummy1, wordsized op2){\
    *(seg0+dest) = op1 operator op2;\
}

MMM_INST(mod , %)     MML_INST(mod , %)       MLL_INST(mod , %)
MMM_INST(div , /)     MML_INST(div , /)       MLL_INST(div , /)
MMM_INST(mul , *)     MML_INST(mul , *)       MLL_INST(mul , *)
MMM_INST(add , +)     MML_INST(add , +)       MLL_INST(add , +)
MMM_INST(sub , -)     MML_INST(sub , -)       MLL_INST(sub , -)
MMM_INST(band, &)     MML_INST(band, &)       MLL_INST(band, &)
MMM_INST(bor , |)     MML_INST(bor , |)       MLL_INST(bor , |)
MMM_INST(bxor, ^)     MML_INST(bxor, ^)       MLL_INST(bxor, ^)
MMM_INST(bshl, <<)    MML_INST(bshl, <<)      MLL_INST(bshl, <<)
MMM_INST(bshr, >>)    MML_INST(bshr, >>)      MLL_INST(bshr, >>)
MMM_INST(land, &&)    MML_INST(land, &&)      MLL_INST(land, &&)
MMM_INST(lor , ||)    MML_INST(lor , ||)      MLL_INST(lor , ||)
MMM_INST(grt , >)     MML_INST(grt , >)       MLL_INST(grt , >)
MMM_INST(geq , >=)    MML_INST(geq , >=)      MLL_INST(geq , >=)
MMM_INST(equ , ==)    MML_INST(equ , ==)      MLL_INST(equ , ==)
MMM_INST(neq , !=)    MML_INST(neq , !=)      MLL_INST(neq , !=)
MMM_INST(les , <)     MML_INST(les , <)       MLL_INST(les , <)
MMM_INST(leq , >=)    MML_INST(leq , >=)      MLL_INST(leq , >=)

#define MM_INST(name, operator) \
void name##_mm(wordsized* seg0, wordsized dest, wordsized* seg1, wordsized op1, wordsized* dummy0, wordsized dummy1){\
    *(seg0+dest) = operator *(seg1+op1);\
}
#define ML_INST(name, operator) \
void name##_ml(wordsized* seg0, wordsized dest, wordsized* dummy0, wordsized op1, wordsized* dummy1, wordsized dummy2){\
    *(seg0+dest) = operator op1;\
}

MM_INST(lnot, !)       ML_INST(lnot, !)
MM_INST(bnot, ~)       ML_INST(bnot, ~)
MM_INST(bneg, -)       ML_INST(bneg, -)

// mov instructions
MM_INST(mov , )       ML_INST(mov , )

// load instructions
void load_mm(wordsized* seg0, wordsized dest, wordsized* seg1, wordsized op1, wordsized* seg2, wordsized dummy0){
    *(seg0+dest) = *(seg2+*(seg1+op1));
}
void load_ml(wordsized* seg0, wordsized dest, wordsized* seg1, wordsized op1, wordsized* dummy0, wordsized dummy1){
    *(seg0+dest) = *(seg1+op1);
}

// jump instructions
void jmp_c(wordsized* seg0, wordsized condition, wordsized* dummy0, wordsized addr, wordsized* dummy1, wordsized dummy2){
    if(*(seg0+condition)){
        reg_ip = addr;
    }
}
void jmp_u(wordsized* dummy0, wordsized addr, wordsized* dummy1, wordsized dummy2, wordsized* dummy3, wordsized dummy4){
    reg_ip = addr;
}
// memalloc

/*void add_ds(){
    if(reg_ds){
        reg_ds = realloc(1, 32);
    }
    else{
        reg_ds = calloc(1, 32);
    }
    if (reg_ds)
        ds_size+=32;
    else
        //error; set a flag probably
        ;
}*/ //add later

// syscalls
void rget(wordsized* seg0, wordsized dest, wordsized* dummy0, wordsized size, wordsized* dummy1, wordsized dummy2){   /* ***very unsafe instruction, defeats isolation thing*** */
    read(0, seg0+dest, size);
}
void rput(wordsized* seg0, wordsized src, wordsized* dummy0, wordsized size, wordsized* dummy1, wordsized dummy2){   /* ***very unsafe instruction, defeats isolation thing*** */
    write(1, seg0+src, size);
}
// stack
void push_m(wordsized* seg0, wordsized src, wordsized* dummy0, wordsized dummy1, wordsized* dummy2, wordsized dummy3){
    ++reg_sp;
    *(reg_ss+reg_sp) = *(seg0+src);
}
void push_l(wordsized* dummy0, wordsized src, wordsized* dummy1, wordsized dummy2, wordsized* dummy3, wordsized dummy4){
    ++reg_sp;
    *(reg_ss+reg_sp) = src;
}
void pop_m(wordsized* seg0, wordsized dest, wordsized* dummy0, wordsized dummy1, wordsized* dummy2, wordsized dummy3){
    *(seg0+dest) = *(reg_ss+reg_sp);
    --reg_sp;
}
void pop_l(wordsized* dummy0, wordsized dummy1, wordsized* dummy2, wordsized dummy3, wordsized* dummy4, wordsized dummy5){
    --reg_sp;
}
// registers
void load_ip(wordsized* seg0, wordsized dest, wordsized* dummy0, wordsized dummy1, wordsized* dummy2, wordsized dummy3){
    *(seg0+dest) = reg_ip;
}
void load_sp(wordsized* seg0, wordsized dest, wordsized* dummy0, wordsized dummy1, wordsized* dummy2, wordsized dummy3){
    *(seg0+dest) = reg_sp;
}
void add_sp_m(wordsized* seg0, wordsized src, wordsized* dummy0, wordsized dummy1, wordsized* dummy2, wordsized dummy3){
    reg_sp += *(seg0+src);
}
void add_sp_l(wordsized* dummy0, wordsized src, wordsized* dummy1, wordsized dummy2, wordsized* dummy3, wordsized dummy4){
    reg_sp += src;
}
void sub_sp_m(wordsized* seg0, wordsized src, wordsized* dummy0, wordsized dummy1, wordsized* dummy2, wordsized dummy3){
    reg_sp -= *(seg0+src);
}
void sub_sp_l(wordsized* dummy0, wordsized src, wordsized* dummy1, wordsized dummy2, wordsized* dummy3, wordsized dummy4){
    reg_sp -= src;
}

void (*inst_set_ptr[80])(wordsized*, wordsized, wordsized*, wordsized, wordsized*, wordsized);

void init_inst_set_ptr(){
    inst_set_ptr[INST_MOV_MM]  = &mov_mm  ; inst_set_ptr[INST_MOV_ML]  = &mov_ml  ;
    inst_set_ptr[INST_LOAD_MM] = &load_mm ; inst_set_ptr[INST_LOAD_ML] = &load_ml ;

    inst_set_ptr[INST_MOD_MMM] = &mod_mmm ; inst_set_ptr[INST_MOD_MML] = &mod_mml ; inst_set_ptr[INST_MOD_MLL] = &mod_mll ;
    inst_set_ptr[INST_DIV_MMM] = &div_mmm ; inst_set_ptr[INST_DIV_MML] = &div_mml ; inst_set_ptr[INST_DIV_MLL] = &div_mll ;
    inst_set_ptr[INST_MUL_MMM] = &mul_mmm ; inst_set_ptr[INST_MUL_MML] = &mul_mml ; inst_set_ptr[INST_MUL_MLL] = &mul_mll ;
    inst_set_ptr[INST_ADD_MMM] = &add_mmm ; inst_set_ptr[INST_ADD_MML] = &add_mml ; inst_set_ptr[INST_ADD_MLL] = &add_mll ;
    inst_set_ptr[INST_SUB_MMM] = &sub_mmm ; inst_set_ptr[INST_SUB_MML] = &sub_mml ; inst_set_ptr[INST_SUB_MLL] = &sub_mll ;

    inst_set_ptr[INST_BAND_MMM] = &band_mmm ; inst_set_ptr[INST_BAND_MML] = &band_mml ; inst_set_ptr[INST_BAND_MLL] = &band_mll ;
    inst_set_ptr[INST_BOR_MMM]  = &bor_mmm  ; inst_set_ptr[INST_BOR_MML]  = &bor_mml  ; inst_set_ptr[INST_BOR_MLL]  = &bor_mll  ;
    inst_set_ptr[INST_BXOR_MMM] = &bxor_mmm ; inst_set_ptr[INST_BXOR_MML] = &bxor_mml ; inst_set_ptr[INST_BXOR_MLL] = &bxor_mll ;
    inst_set_ptr[INST_BSHL_MMM] = &bshl_mmm ; inst_set_ptr[INST_BSHL_MML] = &bshl_mml ; inst_set_ptr[INST_BSHL_MLL] = &bshl_mll ;
    inst_set_ptr[INST_BSHR_MMM] = &bshr_mmm ; inst_set_ptr[INST_BSHR_MML] = &bshr_mml ; inst_set_ptr[INST_BSHR_MLL] = &bshr_mll ;

    inst_set_ptr[INST_LAND_MMM] = &land_mmm; inst_set_ptr[INST_LAND_MML] = &land_mml; inst_set_ptr[INST_LAND_MLL] = &land_mll;
    inst_set_ptr[INST_LOR_MMM]  = &lor_mmm ; inst_set_ptr[INST_LOR_MML]  = &lor_mml ; inst_set_ptr[INST_LOR_MLL]  = &lor_mll ;
    inst_set_ptr[INST_GRT_MMM]  = &grt_mmm ; inst_set_ptr[INST_GRT_MML]  = &grt_mml ; inst_set_ptr[INST_GRT_MLL]  = &grt_mll ;
    inst_set_ptr[INST_GEQ_MMM]  = &geq_mmm ; inst_set_ptr[INST_GEQ_MML]  = &geq_mml ; inst_set_ptr[INST_GEQ_MLL]  = &geq_mll ;
    inst_set_ptr[INST_EQU_MMM]  = &equ_mmm ; inst_set_ptr[INST_EQU_MML]  = &equ_mml ; inst_set_ptr[INST_EQU_MLL]  = &equ_mll ;
    inst_set_ptr[INST_NEQ_MMM]  = &neq_mmm ; inst_set_ptr[INST_NEQ_MML]  = &neq_mml ; inst_set_ptr[INST_NEQ_MLL]  = &neq_mll ;
    inst_set_ptr[INST_LES_MMM]  = &les_mmm ; inst_set_ptr[INST_LES_MML]  = &les_mml ; inst_set_ptr[INST_LES_MLL]  = &les_mll ;
    inst_set_ptr[INST_LEQ_MMM]  = &leq_mmm ; inst_set_ptr[INST_LEQ_MML]  = &leq_mml ; inst_set_ptr[INST_LEQ_MLL]  = &leq_mll ;

    inst_set_ptr[INST_BNOT_MM]  = &bnot_mm ; inst_set_ptr[INST_BNOT_ML]  = &bnot_ml ;
    inst_set_ptr[INST_BNEG_MM]  = &bneg_mm ; inst_set_ptr[INST_BNEG_ML]  = &bneg_ml ;
    inst_set_ptr[INST_LNOT_MM]  = &lnot_mm ; inst_set_ptr[INST_LNOT_ML]  = &lnot_ml ;

    inst_set_ptr[INST_JMP_C]    = &jmp_c   ; inst_set_ptr[INST_JMP_U]    = &jmp_u   ;

    inst_set_ptr[INST_RGET]     = &rget    ; inst_set_ptr[INST_RPUT]     = &rput    ;

    inst_set_ptr[INST_PUSH_M]   = &push_m  ; inst_set_ptr[INST_PUSH_L]   = &push_l  ;
    inst_set_ptr[INST_POP_M]    = &pop_m   ; inst_set_ptr[INST_POP_L]    = &pop_l   ;

    inst_set_ptr[INST_LOAD_SP]  = &load_sp  ; inst_set_ptr[INST_LOAD_IP]  = &load_ip  ;
    inst_set_ptr[INST_ADD_SP_M] = &add_sp_m ; inst_set_ptr[INST_ADD_SP_L] = &add_sp_l ;
    inst_set_ptr[INST_SUB_SP_M] = &sub_sp_m ; inst_set_ptr[INST_SUB_SP_L] = &sub_sp_l ;
}