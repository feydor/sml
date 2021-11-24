#ifndef PROTOTYPE_H
#define PROTOTYPE_H
#include <vector>
#include <memory>
#include <string>
#include "llvm-includes.h"
#include "expr.h"

class SMOL;
class ExprAST;

// DeclarationAST - This class represents either a function definition
// or a prototype without a definition
class DeclarationAST {
    public:
        virtual ~DeclarationAST() = default;
        virtual llvm::Function *code_gen(SMOL &smol) = 0;
        virtual void compile(SMOL& compiler) = 0;
        virtual const std::string &get_name() const = 0;
        virtual const std::vector<std::string> &get_args() const = 0;
};


/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
/// NOTE:
/// Since all values are double precision floating point, the type of each argument
/// doesn’t need to be stored anywhere. In a more aggressive and realistic language,
/// the “ExprAST” class would probably have a type field.
class PrototypeAST : public DeclarationAST {
    public:
    PrototypeAST(const std::string &name, std::vector<std::string> args)
        : name(name), args(std::move(args)) {}

    const std::string &get_name() const override;
    const std::vector<std::string> &get_args() const override;

    llvm::Function *code_gen(SMOL &smol) override;
    void compile(SMOL& compiler) override;

    private:
    std::string name;
    std::vector<std::string> args;
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST : public DeclarationAST {
    public:
    FunctionAST(std::unique_ptr<PrototypeAST> prototype,
              std::unique_ptr<ExprAST> body)
    : prototype(std::move(prototype)), body(std::move(body)) {}

    const std::string &get_name() const override;
    const std::vector<std::string> &get_args() const override;
    llvm::Function *code_gen(SMOL &smol) override;
    void compile(SMOL& compiler) override;

    private:
    std::unique_ptr<PrototypeAST> prototype;
    std::unique_ptr<ExprAST> body;
};


#endif