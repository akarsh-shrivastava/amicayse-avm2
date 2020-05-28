#ifndef PARSER_H_INCLUDED
    #define PARSER_H_INCLUDED
    #include <amicayse/parsegen.h>
    #include "tokentype.h"
    extern std::vector<Token> tokens;

    enum amicayse::NonTerminal : int {
        s=std::numeric_limits<int>::min(),
        inst, inst_,
        ops, ops_, op,
        numconst
    };

    class Parser{
        ParseGen parsegen;
        public:
        Parser();
        ParseTreeNode* get_tree();
    };
#endif