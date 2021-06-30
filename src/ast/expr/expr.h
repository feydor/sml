#ifndef SMOL_EXPR_H
#define SMOL_EXPR_H
#include "token.h"
#include "object.h"
#include <vector>


namespace Ast {

    class Expr {
        public:
            virtual Obj::Object* eval() = 0;
            virtual std::string to_str() const = 0;
            virtual ~Expr() = default;
    };

    class Binary : public Expr {
        public:
            Binary(Expr* left, Tok op, Expr* right)
                : op_(op), left_(left), right_(right) {};
            ~Binary() override;
            Obj::Object* eval() override;
            std::string to_str() const override;
        private:
            Tok op_;
            Expr* left_;
            Expr* right_;
    };

    class Unary : public Expr {
        public:
            Unary(Tok op, Expr* right) : op_(op), right_(right) {};
            ~Unary() override;
            Obj::Object* eval() override;
            std::string to_str() const override;
        private:
            Tok op_;
            Expr* right_;
    };

    class Ternary : public Expr {
        public:
            Ternary(Expr* cond, Tok op, Expr* iftrue, Expr* iffalse)
                : cond_(cond), op_(op), iftrue_(iftrue), iffalse_(iffalse) {};
            ~Ternary() override;
            Obj::Object* eval() override;
            std::string to_str() const override;
        private:
            Expr* cond_;
            Tok op_;
            Expr* iftrue_;
            Expr* iffalse_;
    };

    // Conditional expression: and, or, comparison
    class Cond : public Expr {
        public:
            Cond(Expr* left, Tok op, Expr* right)
                : left_(left), op_(op), right_(right) {};
            ~Cond() override;
            Obj::Object* eval() override;
            std::string to_str() const override;

        private:
            Expr* left_;
            Tok op_;
            Expr* right_;
    };

    // Assignment expression: a = b, evals to b
    class Asgmt : public Expr {
        public:
            Asgmt(Expr* left, Expr* right) : left_(left), right_(right) {};
            ~Asgmt() override;
            Obj::Object* eval() override;
            std::string to_str() const override;
        private:
            Expr* left_;
            Expr* right_;
    };

    // Literal expression: val: string, num, bool, nil, arr, hash
    class Literal : public Expr {
        public:
            explicit Literal(Obj::Object* val) : val_(val) {};
            Obj::Object* eval() override;
            std::string to_str() const override;
            ~Literal() override;
        private:
            Obj::Object* val_;
    };

    class Var : public Expr {
        public:
            explicit Var(std::string name) : name_(std::move(name)) {};
            Obj::Object* eval() override;
            std::string to_str() const override;
        private:
            std::string name_;
    };

    class FnExpr : public Expr {
        public:
            explicit FnExpr(std::string name) : name_(std::move(name)) {};
            ~FnExpr() override;
            Obj::Object* eval() override;
            std::string to_str() const override;
            void add_arg(Expr* expr);
        private:
            std::vector<Expr*> args_;
            std::string name_;
    };

    class Arr : public Expr {
        public:
            explicit Arr(std::vector<Expr*> exprs) : exprs_(std::move(exprs)) {};
            ~Arr() override;
            Obj::Object* eval() override;
            std::string to_str() const override;
            void add_expr(Expr* expr);
        private:
            std::vector<Expr*> exprs_;
    };
}
#endif
