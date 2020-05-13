#ifndef TOKENTYPE_H_DECLARED
    #define TOKENTYPE_H_DECLARED
    #include <amicayse/utils.h>
    using namespace amicayse;
    enum amicayse::TokenType : int {
        DELIMITOR,
        HEXCONST,
        DECCONST,
        STRCONST,
        CHRCONST,

        OBKT,
        CBKT,
        OBRC,
        CBRC,
        OPAR,
        CPAR,

        COMMA,
        COLON,

        DOLLARSIGN,
        REGISTER,

        IDENTIFIER,


        UNKNOWN,
        EPSILON,
        DOLLAR
    };
#endif