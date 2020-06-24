#ifndef TOKENTYPE_H_DECLARED
    #define TOKENTYPE_H_DECLARED
    #include <amicayse/utils.h>
    using namespace amicayse;
    enum amicayse::TokenType : int {
        DELIMITOR,      // 0
        HEXCONST,       // 1
        DECCONST,       // 2
        STRCONST,       // 3
        CHRCONST,       // 4

        OBKT,           // 5
        CBKT,           // 6
        OBRC,           // 7
        CBRC,           // 8
        OPAR,           // 9
        CPAR,           // 10

        COMMA,          // 11
        COLON,          // 12

        DOLLARSIGN,     // 13
        REGISTER,       // 14

        INSTRUCTION,    // 15
        IDENTIFIER,     // 16


        SPECIAL,        // 17
        UNKNOWN,        // 18
        EPSILON,        // 19
        DOLLAR          // 20
    };
#endif