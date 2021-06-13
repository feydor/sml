#ifndef SYM_H
#define SYM_H
#include "stmt.h"
#include "value.h"
#include <unordered_map>
#include <vector>

// Symbols are user-defined identifiers with definitions
enum class Sym_t{ FN, VAR };
struct Sym {
    Sym_t type;
    std::string sym;

    Sym(Sym_t type, std::string sym)
        : type(type), sym(sym) {};
};

// Vars are user-defined identifiers with a value
struct Var : public Sym {
    Val::Val val;
    
    // transfer ownership constructor
    /*
    Var(Sym sym, Val::Val val)
        : Sym(sym), val(val) {};
        */

    // NUM var
    Var(std::string sym, double num)
        : Sym(Sym_t::VAR, sym), val(Val::Val(num)) {};

    // STR var
    Var(std::string sym, std::string str)
        : Sym(Sym_t::VAR, sym), val(Val::Val(str)) {};

    // BOOL var
    Var(std::string sym, bool b)
        : Sym(Sym_t::VAR, sym), val(Val::Val(b)) {};
};

// Functions are user-defined identifiers with a list of statements
struct Fn : public Sym {
    std::vector<Ast::Stmt *> stmts;

    Fn(std::string sym, std::vector<Ast::Stmt *> stmts)
        : Sym(Sym_t::FN, sym), stmts(stmts) {};
};

// a table that holds all of the current syms
// and their definitions, if defined
class SymTable {
    std::unordered_map<std::string, Sym *> table;

    public:
    void insert(Var *var);
    bool in_table(Sym const &sym) const;
    Sym *get(Sym const &sym) const;
};

#endif

