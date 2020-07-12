#ifndef TRANSFORMER_H_INCLUDED
    #define TRANSFORMER_H_INCLUDED
    #include "tokentype.h"
    #include "parser.h"
    class Transformer{
        ParseTreeNode *root;
        std::vector<ParseTreeNode*> transformS(ParseTreeNode*);
        ParseTreeNode* transformInst(ParseTreeNode*);
        ParseTreeNode* transformInst_(ParseTreeNode*);
        std::vector<ParseTreeNode*> transformOps(ParseTreeNode*);
        std::vector<ParseTreeNode*> transformOps_(ParseTreeNode*);
        ParseTreeNode* transformOp(ParseTreeNode*);
        ParseTreeNode* transformNumconst(ParseTreeNode*);
        
        public:
        Transformer(ParseTreeNode*);
        ParseTreeNode* get_ast();
    };
#endif