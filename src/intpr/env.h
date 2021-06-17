#ifndef SMOL_ENV_H
#define SMOL_ENV_H
#include "sym.h"
#include <memory>

// a table that holds all of the current syms
// and their definitions, if defined
class Env {
    std::unordered_map<std::string, std::shared_ptr<Var>> sym_table;
    public: 
    void insert_var(std::shared_ptr<Var> var);
    void replace_var(std::shared_ptr<Var> var);
    bool in_env(Sym const &sym) const;
    std::shared_ptr<Var> get(Sym const &sym) const;

};

#endif

