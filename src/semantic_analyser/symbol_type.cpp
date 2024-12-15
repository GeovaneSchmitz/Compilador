#include "symbol_type.hpp"

namespace semantic_analyser {

std::string toString(SymbolType type) {
    switch (type) {
    case SymbolType::VARIABLE:
        return "VARIABLE";
    case SymbolType::FUNCTION:
        return "FUNCTION";
    }
    return "INVALID";
}

} // namespace semantic_analyser