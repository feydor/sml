#ifndef SYM_H
#define SYM_H
#include "stmt.h"
#include "value.h"
#include <unordered_map>
#include <vector>

// Symbols are user-defined identifiers with definitions
enum class Sym_t{ FN, VAR };
class Sym {
    Sym_t type_;
    std::string sym_;

    public:
    Sym(Sym_t type, std::string sym)
        : type_(type), sym_(sym) {};

    Sym_t type() const;
    std::string sym() const;
};

// Vars are user-defined identifiers with a value
class Var : public Sym {
    Val::Val val_;
    
    public:
    // NUM var
    Var(std::string sym, double num)
        : Sym(Sym_t::VAR, sym), val_(Val::Val(num)) {};

    // STR var
    Var(std::string sym, std::string str)
        : Sym(Sym_t::VAR, sym), val_(Val::Val(str)) {};

    // BOOL var
    Var(std::string sym, bool b)
        : Sym(Sym_t::VAR, sym), val_(Val::Val(b)) {};

    Val::Val val() const;
};

// Functions are user-defined identifiers with a list of statements
class Fn : public Sym {
    // TODO: Change to Block type
    std::vector<Ast::Stmt *> stmts;

    public:
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

