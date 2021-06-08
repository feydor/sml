#ifndef SYM_H
#define SYM_H

struct Sym {
    enum sym_t { FN, VAR } type;
    union {
        double num;
        std::string str;
        bool b;
    };
    Sym *next;
};

class SymTable {
    Sym *head = nullptr;
    int size = 0;
};


#endif

