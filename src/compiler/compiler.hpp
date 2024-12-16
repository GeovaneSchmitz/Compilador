#pragma once

#include "../lexical_analyser/lexical_analyser.hpp"
#include "../syntactic_analyser/syntactic_analyser.hpp"
#include "../semantic_analyser/semantic_analyser.hpp"
#include "../log/log.hpp"
#include <string>
#include <iostream>

using lexical_analyser::LexicalAnalyser;
using syntactic_analyser::SyntacticAnalyser;
using semantic_analyser::SemanticAnalyser;
using cmp_log::Log;

namespace xpp_compiler {

    class Compiler {
        public:
            Compiler();
            Compiler(LexicalAnalyser* lexical_analyser, SyntacticAnalyser* syntactic_analyser, SemanticAnalyser* semantic_analyser);
            ~Compiler();

            int compile(const char* source);

        private:
            LexicalAnalyser* lexical_analyser_;
            SyntacticAnalyser* syntactic_analyser_;
            SemanticAnalyser* semantic_analyser_;
            Log log_;

    };

} // namespace xpp_compiler