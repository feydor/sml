#ifndef SMOL_STMT_H
#define SMOL_STMT_H
#include "expr.h"
#include <string>
#include <vector>
#include <memory>

namespace Ast {
    class Expr;

    class Stmt {
        public:
        virtual void exec() = 0;
        virtual ~Stmt() = default;
    };

    class ExprStmt : public Stmt {
        public:
        ExprStmt(Ast::Expr *expr)
            : expr_(expr) {};
        void exec() override;
        ~ExprStmt() override;

        private:
        Ast::Expr* expr_;
    };

    class SayStmt : public Stmt {
        public:
        SayStmt(Ast::Expr* expr)
            : expr_(expr) {};
        void exec() override;
        ~SayStmt() override;

        private:
        Ast::Expr* expr_;
    };

     class BlockStmt : public Stmt {
        public:
        BlockStmt() {};
        void exec() override;
        ~BlockStmt() override;

        // TODO: friend with parser
        void add_stmt(Stmt *stmt);

        private:
        std::vector<Stmt *> stmts_;
    };

    class IfStmt : public Stmt {
        public:
        IfStmt(Ast::Expr* cond, Stmt* body)
            : cond_(cond), body_(body) {};
        void exec() override;
        ~IfStmt() override;
        void add_else(Stmt* els);

        private:
        Ast::Expr* cond_;
        Stmt* body_;
        std::vector<Stmt *> elses_;
    };

    class AsgmtStmt : public Stmt {
        public:
        AsgmtStmt(std::string name, Ast::Expr* expr)
            : name_(name), expr_(expr) {};
        void exec() override;
        ~AsgmtStmt() override;

        private:
        std::string name_;
        Ast::Expr* expr_;
    };

    class WhileStmt : public Stmt {
        public:
        WhileStmt(Ast::Expr* cond, Stmt* body)
            : cond_(cond), body_(body) {};
        void exec() override;
        ~WhileStmt() override;

        private:
        Ast::Expr* cond_;
        Stmt* body_;
    };

    class RetStmt : public Stmt {
        public:
        RetStmt(Ast::Expr* expr)
            : expr_(expr) {};
        void exec() override;
        ~RetStmt() override;

        private:
        Ast::Expr* expr_;
    };
}

#endif

