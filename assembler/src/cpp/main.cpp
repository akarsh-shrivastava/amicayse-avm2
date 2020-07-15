#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "parser.h"
#include "transformer.h"
#include "generator.h"

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
    
    Parser parser1;
    ParseTreeNode *pt_node = parser1.get_tree(), *ast_node;
    if(!pt_node){
        exit(-1);
    }

    Transformer transformer1(pt_node);
    ast_node = transformer1.get_ast();
    //ast_node->preorder();
    
    Generator generator1(ast_node);
    generator1.write_code();
    std::string err = generator1.get_error();
    if(err.empty()){
        std::string code = generator1.get_code();
        //std::cout<<code<<std::endl;
        std::ofstream fout(filename+".avm2", std::ios::binary);
        fout.write(code.c_str(), code.size());
    }
    else{
        std::cerr<<err;
    }
    ast_node->delete_all_subtrees();
    delete ast_node;
    return 0;
}