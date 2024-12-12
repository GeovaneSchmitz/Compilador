#include "syntactic_analyzer.hpp"


int main() {
    Grammar grammar = Grammar();

    SyntacticAnalyzer analyzer(grammar);

    std::string entrada = "def ident ( float ident , float ident , float ident , float ident ) { ident = ident + ident * ident + ( ident * ident * ident ) / ident ; print ident ; }";
    analyzer.analyze(entrada);

    std::string entrada2 = "def ident ( float ident , float ident ) { if ( ident > ident ) print ident ; else print ident ; }";
    analyzer.analyze(entrada2);

    return 0;
}