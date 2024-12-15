#pragma once
#include <string>

namespace semantic_analyser {

enum class SymbolType { VARIABLE, FUNCTION };

std::string toString(SymbolType type);

} // namespace semantic_analyser