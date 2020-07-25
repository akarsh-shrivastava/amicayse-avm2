#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "memory.h"
#include "instructions.h"
#include "execute.h"

#define NULLU 0

FILE *logfile;

void sample_prog(){
    *(seg_reg[REG_CS]+0) = INST_MOV_ML;
    *(seg_reg[REG_CS]+1) = REG_DS;
    *(seg_reg[REG_CS]+2) = 0;
    *(seg_reg[REG_CS]+3) = 'H';
    *(seg_reg[REG_CS]+4) = NULLU;

    *(seg_reg[REG_CS]+5) = INST_MOV_ML;
    *(seg_reg[REG_CS]+6) = REG_DS;
    *(seg_reg[REG_CS]+7) = 1;
    *(seg_reg[REG_CS]+8) = 'o';
    *(seg_reg[REG_CS]+9) = NULLU;

    *(seg_reg[REG_CS]+10) = INST_MOV_ML;
    *(seg_reg[REG_CS]+11) = REG_DS;
    *(seg_reg[REG_CS]+12) = 2;
    *(seg_reg[REG_CS]+13) = 'l';
    *(seg_reg[REG_CS]+14) = NULLU;

    *(seg_reg[REG_CS]+15) = INST_MOV_ML;
    *(seg_reg[REG_CS]+16) = REG_DS;
    *(seg_reg[REG_CS]+17) = 3;
    *(seg_reg[REG_CS]+18) = 'a';
    *(seg_reg[REG_CS]+19) = NULLU;

    *(seg_reg[REG_CS]+20) = INST_MOV_ML;
    *(seg_reg[REG_CS]+21) = REG_DS;
    *(seg_reg[REG_CS]+22) = 4;
    *(seg_reg[REG_CS]+23) = '\n';
    *(seg_reg[REG_CS]+24) = NULLU;

    *(seg_reg[REG_CS]+25) = INST_RPUT;
    *(seg_reg[REG_CS]+26) = REG_DS;
    *(seg_reg[REG_CS]+27) = 0;
    *(seg_reg[REG_CS]+28) = 5;
    *(seg_reg[REG_CS]+29) = NULLU;

    *(seg_reg[REG_CS]+30) = INST_EXIT;    
}
void load_program(const char *filename){
    FILE *fptr;
    int len = strlen(filename); 
    if(strcmp(filename+len-5, ".avm2")){
        fprintf(stderr, "Error: Invalid file extension\n");
        exit(-1);
    }
    fptr = fopen(filename, "r");
    if(!fptr){
        fprintf(stderr, "Error: File not found\n");
        exit(-1);
    }
    if(fseek(fptr, 0, SEEK_END)){
        fprintf(stderr, "Error: Error in reading the file\n");
        exit(-1);
    }
    int size = ftell(fptr);
    if(size>258){
        fprintf(stderr, "Too big file\n");
        exit(-1);
    }
    fseek(fptr, 0, SEEK_SET);
    char magic[5]={0};
    for (int i=0; i<4; ++i){
        magic[i] = fgetc(fptr);
    }
    if(strcmp(magic, "avm2")){
        fprintf(stderr, "Error: Unrecognised file format\n");
        exit(-1);
    }
    for(int i=0; true; ++i){
        char ch = fgetc(fptr);
        if(ch==EOF)
            break;
        *(seg_reg[REG_CS]+i) = ch;
    }
}
void main(int argc, const char **argv){
    init_registers();
    init_inst_set_ptr();

    /*printf("%d\n", argc);
    for(int i=1; i<argc; ++i){
        
    }*/
    if(argc==1){
        fprintf(stderr, "Error: No file provided\n");
        exit(-1);
    }
    else if(argc==2);
    else{
        fprintf(stderr, "Error: Too many arguments\n");
        exit(-1);
    }

    load_program(argv[1]);
    //sample_prog();
    logfile = fopen("logs", "w");
    execute(LOG_IP | LOG_DS | LOG_SS | LOG_CS);
    fclose(logfile);
}