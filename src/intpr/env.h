#ifndef SMOL_ENV_H
#define SMOL_ENV_H
#include "sym.h"

// a table that holds all of the current syms
// and their definitions, if defined
class Env {
    std::unordered_map<std::string, Sym *> sym_table;

    public:
    void insert_var(Var *var);
    void replace_var(Var *var);
    bool in_env(Sym const &sym) const;
    Sym *get(Sym const &sym) const;
};

#endif

