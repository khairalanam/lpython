#ifndef LFORTRAN_SEMANTICS_ASR_SCOPES_H
#define LFORTRAN_SEMANTICS_ASR_SCOPES_H

#include <map>

namespace LFortran  {

namespace ASR {
    struct asr_t;
}

struct SymbolTable {
    std::map<std::string, ASR::asr_t*> scope;
    SymbolTable *parent;

    SymbolTable(SymbolTable *parent) : parent{parent} {}

    // Determines a stable hash based on the content of the symbol table
    std::string get_hash();

    // Resolves the symbol `name` recursively in current and parent scopes.
    // Returns `nullptr` if symbol not found.
    ASR::asr_t* resolve_symbol(const std::string &name) {
        if (scope.find(name) == scope.end()) {
            if (parent) {
                return parent->resolve_symbol(name);
            } else {
                return nullptr;
            }
        }
        return scope[name];
    }
};

const int intent_local=0; // local variable (not a dummy argument)
const int intent_in   =1; // dummy argument, intent(in)
const int intent_out  =2; // dummy argument, intent(out)
const int intent_inout=3; // dummy argument, intent(inout)
const int intent_return_var=4; // return variable of a function

static inline bool is_arg_dummy(int intent) {
    return intent == intent_in || intent == intent_out
        || intent == intent_inout;
}

} // namespace LFortran

#endif // LFORTRAN_SEMANTICS_ASR_SCOPES_H
