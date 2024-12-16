#include "symbol.hpp"
namespace semantic_analyser {

Symbol::Symbol(SymbolType type, std::string name)
    : type_(type),
      name_(name) {}

Symbol::~Symbol() {}

SymbolType Symbol::type() const { return type_; }

std::string Symbol::name() const { return name_; }

void Symbol::setInternalRepresentation(std::string ir) { internal_representation = ir; }

std::string Symbol::getInternalRepresentation() const { return internal_representation; }

} // namespace semantic_analyser