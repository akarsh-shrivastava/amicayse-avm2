%{
    #include <vector>
    #include <string>
    #include <fstream>
    #include <iostream>
    #include <algorithm>
    #include <amicayse/parsegen.h>
    #include "tokentype.h"
    extern std::vector<Token> tokens;
    int line_no=1;
%}

DEC [1-9][0-9]*|[0]
HEX 0x[1-9a-fA-F][0-9a-fA-F]*|0


WS  [\ ]|[\t]

%option noyywrap

%%
\/\/([^\n])*\n                 {/*tokens.push_back(Token(SLCOMMENT, YYText(), line_no));*/ ++line_no;}
"/*"([^\*]|\*[^/])*"*/"        {
                                std::string mlc(YYText());
                                //tokens.push_back(Token(MLCOMMENT, mlc, line_no));
                                int no_nl = std::count(mlc.begin(), mlc.end(), '\n');
                                line_no+=no_nl;
                               }
\n                             {tokens.push_back(Token(DELIMITOR, YYText(), line_no++));}

{DEC}                          {tokens.push_back(Token(DECCONST, YYText(), line_no));}
{HEX}                          {tokens.push_back(Token(HEXCONST, YYText(), line_no));}
\"([^"])*\"                    {tokens.push_back(Token(STRCONST, YYText(), line_no));}
'([^'])*'                      {tokens.push_back(Token(CHRCONST, YYText(), line_no));}

\[                             {tokens.push_back(Token(OBKT, YYText(), line_no));}
]                              {tokens.push_back(Token(CBKT, YYText(), line_no));}
\{                             {tokens.push_back(Token(OBRC, YYText(), line_no));}
\}                             {tokens.push_back(Token(CBRC, YYText(), line_no));}
\(                             {tokens.push_back(Token(OPAR, YYText(), line_no));}
\)                             {tokens.push_back(Token(CPAR, YYText(), line_no));}

,                              {tokens.push_back(Token(COMMA, YYText(), line_no));}
:                              {tokens.push_back(Token(COLON, YYText(), line_no));}

\$                             {tokens.push_back(Token(DOLLARSIGN, YYText(), line_no));}
\%[a-zA-Z_][a-zA-Z0-9_]*       {tokens.push_back(Token(REGISTER, YYText(), line_no));}





[a-zA-Z_][a-zA-Z0-9_]*         {tokens.push_back(Token(IDENTIFIER, YYText(), line_no));}

({WS})+                        {}
.                              {tokens.push_back(Token(UNKNOWN, YYText(), line_no));}
%%

void lexer(std::string filename)
{
    std::ifstream fin(filename.c_str());
    yyFlexLexer lexer1(fin, std::cout);

    lexer1.yylex();
}
