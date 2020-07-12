#ifndef GENERATOR_H_INCLUDED
    #define GENERATOR_H_INCLUDED
    #include <regex>
    #include <string>
    #include <utility>
    #include <map>
    #include "tokentype.h"
    class Generator{
        ParseTreeNode *root;
        std::string filename;
        std::map<std::string, unsigned char> label_table;
        std::string error_msg;
        std::string code;
        //unsigned char code_ptr;
        unsigned char label_ptr;

        //std::vector<std::pair<std::string, unsigned char>> inst_regex;
        std::map<std::string, unsigned char> inst_regex;
        std::string suffixes;

        void add_code(std::string, std::string, ParseTreeNode*);
        public:
        Generator(ParseTreeNode*, std::string);
        void write_code();
    };
#endif