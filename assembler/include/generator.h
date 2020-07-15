#ifndef GENERATOR_H_INCLUDED
    #define GENERATOR_H_INCLUDED
    #include <regex>
    #include <string>
    #include <utility>
    #include <map>
    #include "tokentype.h"
    class Generator{
        ParseTreeNode *root;
        std::map<std::string, unsigned char> label_table;
        std::string error_msg;
        std::string code;
        unsigned char label_ptr;
        int child_itr, label_itr;

        std::map<std::string, unsigned char> inst_regex;
        std::string suffixes;

        void add_code(std::string, std::string, ParseTreeNode*);
        unsigned char get_label_addr(std::string);
        unsigned char get_int(ParseTreeNode *p);
        public:
        Generator(ParseTreeNode*);
        void write_code();
        std::string get_code();
        std::string get_error();
    };
#endif