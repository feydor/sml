#include "sym.h"

/* only inserts unique vars */
void
SymTable::insert(Var *v)
{
    this->table.insert({v->sym(), v});
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
