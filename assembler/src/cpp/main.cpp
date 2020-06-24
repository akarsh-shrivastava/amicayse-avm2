#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "parser.h"
#include "transformer.h"

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
    
    Parser parser;
    ParseTreeNode *pt_node = parser.get_tree(), *ast_node;
    if(!pt_node){
        exit(-1);
    }
    pt_node->preorder();
    
    std::cout<<"\n\n\n";

    Transformer t1(pt_node);
    ast_node = t1.get_ast();
    ast_node->preorder();

    //pt_node->delete_all_subtrees();
    ast_node->delete_all_subtrees();
    delete ast_node;
    delete pt_node;

    return 0;
}