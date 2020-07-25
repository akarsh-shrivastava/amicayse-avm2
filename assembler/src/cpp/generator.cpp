#include "generator.h"
extern "C"{
    #include "../../../runtime/include/instructions.h"
    #include "../../../runtime/include/memory.h"
};

unsigned char Generator::get_int(ParseTreeNode *p){
    if (p->terminal->type == CHRCONST){
        std::string ch=p->terminal->lexeme;
        if(ch.size()==3)
            return (unsigned char)(ch[1]);
        else if(ch.size()==4){
            if(ch[1]=='\\'){
                switch(ch[2]){
                    case 'a': return '\a';
                    case 'b': return '\b';
                    case 'e': return '\e';
                    case 'f': return '\f';
                    case 'n': return '\n';
                    case 'r': return '\r';
                    case 't': return '\t';
                    case 'v': return '\v';
                    case '0': return '\0';
                }
                error_msg+="Error at line "+std::to_string(p->terminal->line)
                         + ": Invalid character constant "+ch+"\n";
                return 0;
            }
            else{
                error_msg+="Error at line "+std::to_string(p->terminal->line)
                         + ": Invalid character constant "+ch+"\n";
                return 0;
            }
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)
                     + ": Invalid character constant "+ch+"\n";
            return 0;
        }
    
    }
    else
        return (unsigned char)std::stoull(p->terminal->lexeme, nullptr, 0);;
}

Generator::Generator(ParseTreeNode *r){
    root = r;
    label_ptr = 0;
    code="avm2";
    inst_regex = {
        {std::string("halt"),  INST_EXIT},     //done
        {std::string("mov"),   INST_MOV_MM},   //done
        {std::string("load"),  INST_LOAD_MM},  //done
        {std::string("mod"),   INST_MOD_MMM},  //done
        {std::string("div"),   INST_DIV_MMM},  //done
        {std::string("mul"),   INST_MUL_MMM},  //done
        {std::string("add"),   INST_ADD_MMM},  //done
        {std::string("sub"),   INST_SUB_MMM},  //done
        {std::string("band"),  INST_BAND_MMM}, //done
        {std::string("bor") ,  INST_BOR_MMM},  //done
        {std::string("bxor"),  INST_BXOR_MMM}, //done
        {std::string("bshl"),  INST_BSHL_MMM}, //done
        {std::string("bshr"),  INST_BSHR_MMM}, //done
        {std::string("land"),  INST_LAND_MMM}, //done
        {std::string("lor") ,  INST_LOR_MMM},  //done
        {std::string("grt"),   INST_GRT_MMM},  //done
        {std::string("geq"),   INST_GEQ_MMM},  //done
        {std::string("equ"),   INST_EQU_MMM},  //done
        {std::string("neq"),   INST_NEQ_MMM},  //done
        {std::string("les"),   INST_LES_MMM},  //done
        {std::string("leq"),   INST_LEQ_MMM},  //done
        {std::string("bnot"),  INST_BNOT_MM},  //done
        {std::string("bneg"),  INST_BNEG_MM},  //done
        {std::string("lnot"),  INST_LNOT_MM},  //done
        {std::string("jmp"),   INST_JMP_C},    //done
        {std::string("rget"),  INST_RGET},     //done
        {std::string("rput"),  INST_RPUT},     //done
        {std::string("push"),  INST_PUSH_M},   //done
        {std::string("pop") ,  INST_POP_M},    //done
        {std::string("loadsp"),INST_LOAD_SP},  //done
        {std::string("loadip"),INST_LOAD_IP},  //done
        {std::string("addsp"), INST_ADD_SP_M}, //done
        {std::string("subsp"), INST_SUB_SP_M}  //done
    };
    suffixes = std::string("b");
}

std::string Generator::get_code(){
    return code;
}
std::string Generator::get_error(){
    return error_msg;
}

void Generator::write_code(){
    int size = root->children.size();
    int lbl = 0;
    for (int i=0; i<size; ++i){
        ParseTreeNode *node = (ParseTreeNode*)root->children[i];
        if (node->terminal->type == COLON){
            ParseTreeNode *label_node = (ParseTreeNode*)(node->children[0]);
            try{
                label_table.at(label_node->terminal->lexeme);
                error_msg+="Error at line "+std::to_string(label_node->terminal->line)+": Multiple definitions of the same label\n";
            }
            catch(std::out_of_range ex){
                label_table[label_node->terminal->lexeme] = lbl;
            }
        }
        else if(node->terminal->type == IDENTIFIER){
            lbl+=7;
        }
    }
    for(int i=0; i<size; ++i){
        ParseTreeNode *node = (ParseTreeNode*)root->children[i];
        if (node->terminal->type == COLON){}
        else if(node->terminal->type == IDENTIFIER){
            bool notFound = true;
            for (std::map<std::string, unsigned char>::iterator itr = inst_regex.begin(); itr != inst_regex.end(); ++itr){
                std::smatch match, suffix_match;
                if(std::regex_search(node->terminal->lexeme, match, std::regex(itr->first))){
                    if(match.prefix() != "")
                        continue;
                    std::string s1 = match.suffix().str();
                    if(std::regex_match(s1, suffix_match, std::regex(suffixes))){
                        add_code(match.str(), s1, node);
                        notFound = false;
                    }
                    else{
                        if((match.str()=="load" && (match.suffix()=="ipb" || match.suffix()=="spb") ) ||
                          ( (match.str()=="add" || match.str()=="sub") && match.suffix()=="sp"))
                            continue;
                        error_msg+="Error at line "+std::to_string(node->terminal->line)+": Invalid suffix for instruction "+match.str()+". Allowed suffixes are `b`\n";
                    }
                }
            }
            if(notFound)
                error_msg+="Error at line "+std::to_string(node->terminal->line)+": Invalid instruction "+node->terminal->lexeme+"\n";
        }
        else{
            error_msg+="Error at line "+std::to_string(node->terminal->line)
                     +": Invalid line, only instructions and label declarations allowed\n";
        }
    }
}

void Generator::add_code(std::string inst, std::string suffix, ParseTreeNode* p){
    unsigned char op_code = inst_regex[inst], off=0,
                  seg0=0, op0=0, seg1=0, op1=0, seg2=0, op2=0;

    ParseTreeNode *ch0=NULL, *ch1=NULL, *ch2=NULL;
    if (inst == "halt")
    {
        if(p->children.size() != 0){
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" should have no arguments, "+std::to_string(p->children.size())+" arguments given\n";
            return;
        }
    }
    // 3 arg albo
    else if(inst=="mod" || inst=="div" || inst=="mul" || inst=="add" || inst=="sub" || 
            inst=="band"|| inst=="bor" || inst=="bxor"|| inst=="bshl"|| inst=="bshr"|| 
            inst=="land"|| inst=="lor" ||
            inst=="grt" || inst=="geq" || inst=="equ" || inst=="neq" || inst=="leq" || inst=="les") 
    {
        // size check
        if(p->children.size() != 3){
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 3 arguments, "+std::to_string(p->children.size())+" given\n";
            return;
        }
        ch0 = (ParseTreeNode*)(p->children[0]);
        ch1 = (ParseTreeNode*)(p->children[1]);
        ch2 = (ParseTreeNode*)(p->children[2]);

        // decode op2
        if(ch2->terminal->type == OPAR){
            seg2 = REG_DS;
        }
        else if(ch0->terminal->type == OBKT){
            seg2 = REG_SS;
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 3rd argument to be a memory location\n";
            return;
        }
        op2 = get_int((ParseTreeNode*)(ch2->children[0]));

        // decode op0 & op1
        if(ch1->terminal->type == OPAR || ch1->terminal->type == OBKT){
            if(ch1->terminal->type == OPAR){
                seg1 = REG_DS;
            }
            else{
                seg1 = REG_SS;
            }
            if(ch0->terminal->type == OPAR){
                seg0 = REG_DS;
            }
            else if(ch0->terminal->type == OBKT){
                seg0 = REG_SS;
            }
            else if(ch0->terminal->type == DOLLARSIGN){
                off=1;
            }
            else{
                error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                         + inst+" requires 1st argument to be a memory location or a literal\n";
                return;
            }
        }
        else if(ch1->terminal->type == DOLLARSIGN){
            if(ch0->terminal->type == DOLLARSIGN)
                off=2;
            else if(ch0->terminal->type == OPAR || ch0->terminal->type == OBKT){
                error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                         + inst+" doesn't support memory, literal, memory arguments\n";
                return;
            }
            else{
                error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                         + inst+" requires 1st argument to be a memory location or a literal\n";
                return;
            }
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 2st argument to be a memory location or a literal\n";
            return;
        }

        op1 = get_int((ParseTreeNode*)(ch1->children[0]));
        op0 = get_int((ParseTreeNode*)(ch0->children[0]));
    }
    // 2 arg lbo & move
    else if (inst=="mov" ||  inst=="bnot"||  inst=="lnot"|| inst=="bneg")
    {
        // size check
        if(p->children.size() != 2){
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 2 arguments, "+std::to_string(p->children.size())+" given\n";
            return;
        }
        ch1 = (ParseTreeNode*)(p->children[0]);
        ch2 = (ParseTreeNode*)(p->children[1]);

        // decode op2
        if(ch2->terminal->type == OPAR){
            seg2 = REG_DS;
        }
        else if(ch1->terminal->type == OBKT){
            seg2 = REG_SS;
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 2nd argument to be a memory location\n";
            return;
        }
        op2 = get_int((ParseTreeNode*)(ch2->children[0]));

        // decode op1
        if(ch1->terminal->type == OPAR){
            seg1 = REG_DS;
        }
        else if(ch1->terminal->type == OBKT){
            seg1 = REG_SS;
        }
        else if(ch1->terminal->type == DOLLARSIGN){
            off=1;
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 2nd argument to be a memory location or a literal\n";
            return;
        }
        op1 = get_int((ParseTreeNode*)(ch1->children[0]));
    }
    // get & put
    else if(inst=="rget" ||  inst=="rput")
    {
        // size check
        if(p->children.size() != 2){
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 2 arguments, "+std::to_string(p->children.size())+" given\n";
            return;
        }
        ch1 = (ParseTreeNode*)(p->children[0]);
        ch2 = (ParseTreeNode*)(p->children[1]);

        // decode op1
        if(ch1->terminal->type != DOLLARSIGN){
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 1st argument to be a literal\n";
            return;
        }
        op1 = get_int((ParseTreeNode*)(ch1->children[0]));

        // decode op2
        if(ch2->terminal->type == OPAR){
            seg2 = REG_DS;
        }
        else if(ch2->terminal->type == OBKT){
            seg2 = REG_SS;
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 2nd argument to be a memory location\n";
            return;
        }
        op2 = get_int((ParseTreeNode*)(ch2->children[0]));
    }
    // 0 or 1 arg pop
    else if(inst == "pop")
    {
        // size check and corresponding translation
        if(p->children.size() == 0){
            off=1;
        }
        else if(p->children.size() == 1){
            ch2 = (ParseTreeNode*)(p->children[0]);
            if(ch2->terminal->type == OPAR){
                seg2 = REG_DS;
            }
            else if(ch2->terminal->type == OBKT){
                seg2 = REG_SS;
            }
            else{
                error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                         + inst+" requires 1st argument to be a memory location\n";
                return;
            }
            op2 = get_int((ParseTreeNode*)(ch2->children[0]));
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 0 or 1 arguments, "+std::to_string(p->children.size())+" given\n";
            return;
        }
    }
    // 1 arg push, addsp, subsp
    else if(inst=="push" || inst=="addsp" || inst=="subsp")
    {
        // size check
        if(p->children.size() != 1){
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 1 argument, "+std::to_string(p->children.size())+" given\n";
            return;
        }
        ch2 = (ParseTreeNode*)(p->children[0]);
        
        // decode op2
        if(ch2->terminal->type == OPAR){
            seg2 = REG_DS;
        }
        else if(ch2->terminal->type == OBKT){
            seg2 = REG_SS;
        }
        else if(ch2->terminal->type == DOLLARSIGN){
            off=1;
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 1st argument to be a memory location or literal\n";
            return;
        }
        op2 = get_int((ParseTreeNode*)(ch2->children[0]));
    }
    // 1 arg load sp & ip
    else if(inst=="loadsp" || inst=="loadip")
    {
        // size check
        if(p->children.size() != 1){
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 1 argument, "+std::to_string(p->children.size())+" given\n";
            return;
        }
        ch2 = (ParseTreeNode*)(p->children[0]);
        
        // decode op2
        if(ch2->terminal->type == OPAR){
            seg2 = REG_DS;
        }
        else if(ch2->terminal->type == OBKT){
            seg2 = REG_SS;
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 1st argument to be a memory location\n";
            return;
        }
        op2 = get_int((ParseTreeNode*)(ch2->children[0]));
    }
    // 3 arg all mem load
    else if(inst=="load"){
        // size check
        if(p->children.size() != 3){
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 3 arguments, "+std::to_string(p->children.size())+" given\n";
            return;
        }
        ch0 = (ParseTreeNode*)(p->children[0]);
        ch1 = (ParseTreeNode*)(p->children[1]);
        ch2 = (ParseTreeNode*)(p->children[2]);

        // decode op2
        if(ch2->terminal->type == OPAR){
            seg2 = REG_DS;
        }
        else if(ch2->terminal->type == OBKT){
            seg2 = REG_SS;
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 3rd argument to be a memory location\n";
            return;
        }
        op2 = get_int((ParseTreeNode*)(ch2->children[0]));

        // decode op1
        if(ch1->terminal->type == OPAR){
            seg1 = REG_DS;
        }
        else if(ch1->terminal->type == OBKT){
            seg1 = REG_SS;
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 2nd argument to be a memory location\n";
            return;
        }
        op1 = get_int((ParseTreeNode*)(ch1->children[0]));

        // decode op0
        if(ch0->terminal->type == OPAR){
            seg0 = REG_DS;
        }
        else if(ch0->terminal->type == OBKT){
            seg0 = REG_SS;
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 1st argument to be a memory location\n";
            return;
        }
        op0 = get_int((ParseTreeNode*)(ch0->children[0]));
    }
    // 1 or 2 arg jmp
    else if(inst=="jmp")
    {
        // size check
        if(p->children.size() == 1){
            off=1;
            ch2 = (ParseTreeNode*)(p->children[0]);
            if(ch2->terminal->type == IDENTIFIER){
                op2 = get_label_addr(ch2->terminal->lexeme);
                if(op2 == -1){
                    error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                             + " Undefined label '"+ch2->terminal->lexeme+"'\n";
                    return;
                }
            }
            else{
                error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                         + inst+" requires 1st argument to be a label\n";
                return;
            }
        }
        else if(p->children.size() == 2){
            ch2 = (ParseTreeNode*)(p->children[0]);
            if(ch2->terminal->type == IDENTIFIER){
                op2 = get_label_addr(ch2->terminal->lexeme);
                if(op2 == -1){
                    error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                             + " Undefined label '"+ch2->terminal->lexeme+"'\n";
                    return;
                }
            }
            else{
                error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                         + inst+" requires 1st argument to be a label\n";
                return;
            }

            ch1 = (ParseTreeNode*)(p->children[1]);
            if(ch1->terminal->type == OPAR){
                seg1 = REG_DS;
            }
            else if(ch1->terminal->type == OPAR){
                seg1 = REG_SS;
            }
            else{
                error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                         + inst+" requires 1st argument to be a label\n";
                return;
            }
            op1 = get_int((ParseTreeNode*)(ch1->children[0]));
        }
        else{
            error_msg+="Error at line "+std::to_string(p->terminal->line)+": "
                     + inst+" requires 1 or 2 arguments, "+std::to_string(p->children.size())+" given\n";
            return;
        }
    }

    code.push_back(op_code+off);
    code.push_back( seg2<<4 | seg1<<2 | seg0 );
    code.push_back(op2);
    code.push_back(op1);
    code.push_back(op0);

}

unsigned char Generator::get_label_addr(std::string label){
    try{
        return label_table.at(label);
    }
    catch(std::out_of_range ex){
        return -1;
    }
}
/*for(std::map<std::string, unsigned char>::iterator itr=label_table.begin(); itr!=label_table.end(); ++itr){
    std::cout<<itr->first<<": "<<(unsigned int)itr->second<<std::endl;
}
std::cout<<error_msg;*/
/*
bool std::regex_match(
    const std::__cxx11::basic_string<_Ch_type, _Ch_traits, _Ch_alloc>&&,
    std::__cxx11::match_results<typename std::__cxx11::basic_string<_Ch_type, _Ch_traits, _Ch_alloc>::const_iterator, _Alloc>&, 
    const std::__cxx11::basic_regex<_Ch_type, _Rx_traits>&, 
    std::regex_constants::match_flag_type) 
[with _Ch_traits = std::char_traits<char>; _Ch_alloc = std::allocator<char>; _Alloc = std::allocator<std::__cxx11::sub_match<__gnu_cxx::__normal_iterator<const char*, std::__cxx11::basic_string<char> > > >; _Ch_type = char; _Rx_traits = std::__cxx11::regex_traits<char>; typename std::__cxx11::basic_string<_Ch_type, _Ch_traits, _Ch_alloc>::const_iterator = __gnu_cxx::__normal_iterator<const char*, std::__cxx11::basic_string<char> >]
*/