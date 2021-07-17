#ifndef SMOL_STMT_H
#define SMOL_STMT_H
#include "expr.h"
#include <string>
#include <vector>
#include <memory>

namespace Ast {
    // class Expr;

    class Stmt {
        public:
            virtual void exec() = 0;
            virtual ~Stmt() = default;
    };

    class ExprStmt : public Stmt {
        public:
            explicit ExprStmt(std::unique_ptr<Ast::Expr> expr);
            void exec() override;
            ~ExprStmt() override;

        private:
            std::unique_ptr<Ast::Expr> expr_;
    };

    class SayStmt : public Stmt {
        public:
            explicit SayStmt(std::unique_ptr<Ast::Expr> expr);
            void exec() override;
            ~SayStmt() override;

        private:
            std::unique_ptr<Ast::Expr> expr_;
    };

     class BlockStmt : public Stmt {
        public:
            BlockStmt() {};
            void exec() override;
            ~BlockStmt() override;

            // TODO: friend with parser
            void add_stmt(std::unique_ptr<Ast::Stmt> stmt);

        private:
            std::vector<std::unique_ptr<Ast::Stmt>> stmts_;
    };

    class IfStmt : public Stmt {
        public:
            IfStmt(std::unique_ptr<Ast::Expr> cond, std::unique_ptr<Ast::Stmt> body);
            void exec() override;
            ~IfStmt() override;
            void add_else(std::unique_ptr<Ast::Stmt> els);

        private:
            std::unique_ptr<Ast::Expr> cond_;
            std::unique_ptr<Ast::Stmt> body_;
            std::vector<std::unique_ptr<Ast::Stmt>> elses_;
    };

    class AsgmtStmt : public Stmt {
        public:
            AsgmtStmt(const std::string& name, std::unique_ptr<Ast::Expr> expr);
            void exec() override;
            ~AsgmtStmt() override;

        private:
            std::string name_;
            std::unique_ptr<Ast::Expr> expr_;
    };

    class WhileStmt : public Stmt {
        public:
            WhileStmt(std::unique_ptr<Ast::Expr> cond, std::unique_ptr<Ast::Stmt> body);
            void exec() override;
            ~WhileStmt() override;

        private:
            std::unique_ptr<Ast::Expr> cond_;
            std::unique_ptr<Ast::Stmt> body_;
    };

    class RetStmt : public Stmt {
        public:
            explicit RetStmt(std::unique_ptr<Ast::Expr> expr);
            void exec() override;
            ~RetStmt() override;

        private:
            std::unique_ptr<Ast::Expr> expr_;
    };
}

#endif

