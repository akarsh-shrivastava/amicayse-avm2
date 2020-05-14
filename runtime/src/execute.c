#include <stdio.h>
#include "instructions.h"
#include "memory.h"
#include "execute.h"

extern FILE *logfile;

void log_cs(){
    fprintf(logfile, "CS:");
    for (int i=0; i<255; ++i){
        if(!(i%16)) fprintf(logfile, "\n");
        fprintf(logfile ,"%3u ", *(seg_reg[REG_CS]+i));
    }
}

void log_ds(){
    fprintf(logfile, "\nDS:");
    for (int i=0; i<255; ++i){
        if(!(i%16)) fprintf(logfile, "\n");
        fprintf(logfile ,"%3u ", *(seg_reg[REG_DS]+i));
    }
}

void log_ss(){
    fprintf(logfile, "\nSS:");
    for (int i=0; i<255; ++i){
        if(!(i%16)) fprintf(logfile, "\n");
        fprintf(logfile ,"%3u ", *(seg_reg[REG_SS]+i));
    }
}

void execute(char flags){
    if(flags & LOG_CS)
        log_cs();
    while(*(seg_reg[REG_CS]+reg_ip) != INST_EXIT){
        if(flags & LOG_IP)
            fprintf(logfile, "\n\n\nIP:%u", reg_ip);

        inst_set_ptr[*(seg_reg[REG_CS]+reg_ip)] (
                    seg_reg[*(seg_reg[REG_CS]+reg_ip+1)], *(seg_reg[REG_CS]+reg_ip+2), 
                    seg_reg[*(seg_reg[REG_CS]+reg_ip+3)], *(seg_reg[REG_CS]+reg_ip+4), 
                    seg_reg[*(seg_reg[REG_CS]+reg_ip+5)], *(seg_reg[REG_CS]+reg_ip+6)
        );


        if(flags & LOG_DS)
            log_ds();
        if(flags & LOG_SS)
            log_ss();


        reg_ip+=7;
    }
}