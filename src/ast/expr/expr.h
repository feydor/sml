#ifndef SMOL_EXPR_H
#define SMOL_EXPR_H
#include "token.h"
#include "vartable.h"
#include <vector>


namespace Ast {

    class Expr {
        public:
            virtual Val eval() = 0;
            virtual std::string to_str() const = 0;
            virtual ~Expr() = default;
    };

    class Binary : public Expr {
        public:
            Binary(Expr* left, Tok op, Expr* right)
                : op_(op), left_(left), right_(right) {};
            ~Binary() override;
            Val eval() override;
            std::string to_str() const override;
        private:
            Tok op_;
            Expr* left_;
            Expr* right_;
    };

    class Unary : public Expr {
        public:
            Unary(Tok op, Expr* right)
                : op_(op), right_(right) {};
            ~Unary() override;
            Val eval() override;
            std::string to_str() const override;
        private:
            Tok op_;
            Expr* right_;
    };

    // Conditional expression: and, or, comparison
    class Cond : public Expr {
        public:
            Cond(Expr* left, Tok op, Expr* right)
                : left_(left), op_(op), right_(right) {};
            ~Cond() override;
            Val eval() override;
            std::string to_str() const override;

        private:
            Expr* left_;
            Tok op_;
            Expr* right_;

            template<typename Op>
            Val eval_(Val const &a, Val const &b, Op fn);
    };

    // Assignment expression: a = b, evals to b
    class Asgmt : public Expr {
        public:
            Asgmt(Expr* left, Expr* right)
                : left_(left), right_(right) {};
            ~Asgmt() override;
            Val eval() override;
            std::string to_str() const override;
        private:
            Expr* left_;
            Expr* right_;
    };

    // Literal expression: val: string, num, bool, nil
    class Literal : public Expr {
        public:
            Literal(Val val)
                : val_(std::move(val)) {};
            ~Literal() override;
            Val eval() override;
            std::string to_str() const override;
        private:
            Val val_;
    };

    class Var : public Expr {
        public:
            Var(std::string name)
                : name_(std::move(name)) {};
            Val eval() override;
            std::string to_str() const override;
        private:
            std::string name_;
    };

    class FnExpr : public Expr {
        public:
            FnExpr(std::string name)
                : name_(std::move(name)) {};
            ~FnExpr() override;
            Val eval() override;
            std::string to_str() const override;
            void add_arg(Expr* expr);
        private:
            std::vector<Expr*> args_;
            std::string name_;
    };
}
#endif
