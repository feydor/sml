#include "env.h"
#include <iostream>

/* only inserts unique vars */
void
Env::insert_var(std::shared_ptr<Var> var)
{
    this->sym_table.insert({var->sym(), var});
}

void
Env::replace_var(std::shared_ptr<Var> var)
{
    this->sym_table.insert_or_assign(var->sym(), var);
}

bool
Env::in_env(Sym const &sym) const
{
    return this->sym_table.find(sym.sym()) != sym_table.end()
        ? true : false;
}
    
/* assumes sym is a valid key in table */
std::shared_ptr<Var>
Env::get(Sym const &sym) const
{
    return this->sym_table.find(sym.sym())->second;
}
