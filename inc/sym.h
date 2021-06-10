#ifndef SYM_H
#define SYM_H
#include "stmt.h"
#include "expr.h"
#include <unordered_map>
#include <vector>

// Symbols are user-defined identifiers with definitions
enum class Sym_t{ FN, VAR };
struct Sym {
    Sym_t stype;
    std::string sym;

    Sym(Sym_t type, std::string sym)
        : stype(type), sym(sym) {};
};

// Vars are user-defined identifiers with a value
enum class Var_t{ NUM, STR, BOOL };
struct Var : public Sym {
    Var_t vtype;
    union {
        double val_num;
        std::string val_str;
        bool val_bool;
    };

    // NUM var
    Var(std::string sym, double num)
        : Sym(Sym_t::VAR, sym), vtype(Var_t::NUM), val_num(num) {};

    // STR var
    Var(std::string sym, std::string str)
        : Sym(Sym_t::VAR, sym), vtype(Var_t::STR), val_str(str) {};

    // BOOL var
    Var(std::string sym, bool b)
        : Sym(Sym_t::VAR, sym), vtype(Var_t::BOOL), val_bool(b) {};
};

// Functions are user-defined identifiers with a list of statements
struct Fn : public Sym {
    std::vector<Stmt *> stmts;

    Fn(std::string sym, std::vector<Stmt *> stmts)
        : Sym(Sym_t::FN, sym), stmts(stmts) {};
};

// a table that holds all of the current syms
// and their definitions, if defined
class SymTable {
    std::unordered_map<std::string, Sym *> table;

    public:
    void insert(Var *var);
    bool in_table(std::string sym_name) const;
    Sym *get(std::string sym_name) const;
};

#endif

