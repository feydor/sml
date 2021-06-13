#include "sym.h"

/* only inserts unique vars */
void
SymTable::insert_var(Var *v)
{
    this->table.insert({v->sym(), v});
}

void
SymTable::replace_var(Var *var)
{
    this->table.insert_or_assign(var->sym(), var); 
}

bool
SymTable::in_table(Sym const &sym) const
{
    return this->table.find(sym.sym()) != table.end()
        ? true : false;
}
    
/* assumes sym_name is a valid key in table */
Sym *
SymTable::get(Sym const &sym) const
{
    return this->table.find(sym.sym())->second;
}
