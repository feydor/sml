#ifndef SMOL_EXPR_H
#define SMOL_EXPR_H
#include "token.h"
#include <vector>
#include <memory>

namespace Obj { class Object; }

namespace Ast {
    class Stmt; // forward declaration

    class Expr {
        public:
            virtual std::shared_ptr<Obj::Object> eval() = 0;
            virtual std::string to_str() const = 0;
            virtual ~Expr() = default;
    };

    class Binary : public Expr {
        public:
            Binary(std::unique_ptr<Expr> left, Tok op, std::unique_ptr<Expr> right);
            std::shared_ptr<Obj::Object> eval() override;
            std::string to_str() const override;
        private:
            Tok op_;
            std::unique_ptr<Expr> left_;
            std::unique_ptr<Expr> right_;
    };

    class Unary : public Expr {
        public:
            Unary(Tok op, std::unique_ptr<Expr> right);
            std::shared_ptr<Obj::Object> eval() override;
            std::string to_str() const override;
        private:
            Tok op_;
            std::unique_ptr<Expr> right_;
    };

    class Ternary : public Expr {
        public:
            Ternary(std::unique_ptr<Expr> cond, Tok op, std::unique_ptr<Expr> iftrue,
                std::unique_ptr<Expr> iffalse);
            std::shared_ptr<Obj::Object> eval() override;
            std::string to_str() const override;
        private:
            std::unique_ptr<Expr> cond_;
            Tok op_;
            std::unique_ptr<Expr> iftrue_;
            std::unique_ptr<Expr> iffalse_;
    };

    // Conditional expression: and, or, comparison
    class Cond : public Expr {
        public:
            Cond(std::unique_ptr<Expr> left, Tok op, std::unique_ptr<Expr> right);
            std::shared_ptr<Obj::Object> eval() override;
            std::string to_str() const override;

        private:
            std::unique_ptr<Expr> left_;
            Tok op_;
            std::unique_ptr<Expr> right_;
    };

    // Assignment expression: a = b, evals to b
    class Asgmt : public Expr {
        public:
            Asgmt(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right);
            std::shared_ptr<Obj::Object> eval() override;
            std::string to_str() const override;
        private:
            std::unique_ptr<Expr> left_;
            std::unique_ptr<Expr> right_;
    };

    // Literal expression: val: string, num, bool, nil, arr, hash
    class Literal : public Expr {
        public:
            explicit Literal(std::unique_ptr<Obj::Object> val);
            std::shared_ptr<Obj::Object> eval() override;
            std::string to_str() const override;
        private:
            std::shared_ptr<Obj::Object> val_;
    };

    class Var : public Expr {
        public:
            explicit Var(std::string name);
            std::shared_ptr<Obj::Object> eval() override;
            std::string to_str() const override;
        private:
            std::string name_;
    };

    class FnExpr : public Expr {
        public:
            explicit FnExpr(std::string name);
            std::shared_ptr<Obj::Object> eval() override;
            std::string to_str() const override;
            void add_arg(std::unique_ptr<Expr> expr);
        private:
            std::vector<std::unique_ptr<Expr>> args_;
            std::string name_;
    };

    class MethodExpr : public Expr {
        public:
            explicit MethodExpr(std::string name);
            std::shared_ptr<Obj::Object> eval() override;
            std::string to_str() const override;
            void add_arg(std::unique_ptr<Expr> expr);
        private:
            std::vector<std::unique_ptr<Expr>> args_;
            std::string name_;
    };

    class Arr : public Expr {
        public:
            explicit Arr(std::vector<std::unique_ptr<Expr>> exprs);
            std::shared_ptr<Obj::Object> eval() override;
            std::string to_str() const override;
            void add_expr(std::unique_ptr<Expr> expr);
        private:
            std::vector<std::unique_ptr<Expr>> exprs_;
    };
}
#endif
