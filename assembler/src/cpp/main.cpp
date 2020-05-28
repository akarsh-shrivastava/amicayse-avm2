#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "parser.h"

std::vector<Token> tokens;

void lexer(std::string);

int main(int argc, char** argv)
{
    std::string filename(argv[1]);
    std::size_t dot_pos = filename.rfind(".");

    std::string code="";
    char ch;

    std::ifstream f(argv[1]);
    if(!f)
    {
        std::cerr<<"No such file\n";
        exit(-1);
    }
    f.close();
    if (filename.substr(dot_pos) != ".lasm")
    {
        std::cerr<<"invalid file type\n";
        exit(-1);
    }
    filename = filename.substr(0,dot_pos);

    lexer(filename+".lasm");
    
    Parser p;
    ParseTreeNode *t = p.get_tree();
    if(!t){
        exit(-1);
    }
    

    return 0;
}