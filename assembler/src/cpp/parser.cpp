#include "parser.h"

Parser::Parser(): parsegen(0){
    std::map<NonTerminal, std::vector<std::vector<int>>> rules = {
        {s    ,   {{inst, s},  {EPSILON}}
        },
        {inst ,   {{IDENTIFIER, inst_, DELIMITOR}, {DELIMITOR}}
        },
        {inst_,   {{COLON},  {ops},  {EPSILON}}
        },
        {ops  ,   {{op, ops_}, {EPSILON}}
        },
        {ops_ ,   {{COMMA, op, ops_}, {EPSILON}}
        },
        {op   ,   {{DOLLARSIGN, numconst}, {OPAR, numconst, CPAR}, {OBKT, numconst, CBKT}, {IDENTIFIER}}
        },
        {numconst,{{HEXCONST}, {DECCONST}, {CHRCONST}}
        }
    };

    parsegen.set_rules(rules);
    parsegen.set_epsilon(EPSILON);
    parsegen.set_dollar(DOLLAR);
    parsegen.set_start_symbol(s);
    parsegen.calc_first();
    parsegen.calc_follow();

    parsegen.set_tokens(tokens);

    //parsegen.print_rules();  std::cout<<"\n";
    //parsegen.print_first();  std::cout<<"\n";
    //parsegen.print_follow(); std::cout<<"\n";
    //parsegen.print_table();  std::cout<<"\n";
}

ParseTreeNode* Parser::get_tree(){
    try{
        return parsegen.get_tree();
    }
    catch (ParserException p){
        std::cerr<<p.what();
    }
    return NULL;
}