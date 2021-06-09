#ifndef SYM_H
#define SYM_H
#include <unordered_map>
#include "expr.h"

// Symbols are user-defined identifiers with definitions
struct Sym {
    enum sym_t { FN, VAR } type;
    std::string sym;

    Sym(sym_t type, std::string sym)
        : type(type), sym(sym) {};
};

// Vars are user-defined identifiers with a value
struct Var : public Sym {
    enum var_t { NUM, STR, BOOL } type;
    union {
        double val_num;
        std::string val_str;
        bool val_bool;
    };

    // NUM var
    Var(sym_t type, std::string sym, double num)
        : Sym{type, sym}, type(NUM), val_num(num) {};

    // STR var
    Var(sym_t type, std::string sym, std::string str)
        : Sym{type, sym}, type(STR), val_str(str) {};

    // BOOL var
    Var(sym_t type, std::string sym, bool b)
        : Sym{type, sym}, type(BOOL), val_bool(b) {};
};

// Functions are user-defined identifiers with a list of statements
struct Fn : public Sym {
    std::vector<Stmt *> stmts;

    Fn(sym_t type, std::string sym, std::vector<Stmt *> stmts)
        : Sym{type, sym}, stmts(stmts) {};
};

// a table that holds all of the current syms
// and their definitions, if defined
class SymTable {
    std::unordered_map<std::string, Sym *> table;

    public:
    // TODO: Define operations on SymTable
    // int insert(Sym *sym);
    // Sym *lookup(std::string ident);
};


#endif

