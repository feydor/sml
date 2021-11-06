#ifndef PROTOTYPE_H
#define PROTOTYPE_H
#include <vector>
#include <memory>
#include <string>
#include "expr.h"

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
/// NOTE:
/// Since all values are double precision floating point, the type of each argument
/// doesn’t need to be stored anywhere. In a more aggressive and realistic language,
/// the “ExprAST” class would probably have a type field.
class PrototypeAST {
    public:
    PrototypeAST(const std::string &name, std::vector<std::string> args)
        : name(name), args(std::move(args)) {}

    const std::string &get_name() const { return name; }
    const std::vector<std::string> &get_args() const { return args; }

    public:
    std::string name;
    std::vector<std::string> args;
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    public:
    FunctionAST(std::unique_ptr<PrototypeAST> prototype,
              std::unique_ptr<ExprAST> body)
    : prototype(std::move(prototype)), body(std::move(body)) {}

    std::string get_name() const;
    std::vector<std::string> get_args() const;

    private:
    std::unique_ptr<PrototypeAST> prototype;
    std::unique_ptr<ExprAST> body;
};


#endif