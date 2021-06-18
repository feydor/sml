#ifndef STMT_H
#define STMT_H
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

        private:
            std::vector<Stmt *> stmts_;
            void add_stmt(Stmt *stmt);
    };

    class IfStmt : public Stmt {
        public:
            IfStmt(Ast::Expr* cond, Stmt* ifstmt, Stmt* else_stmt)
                : cond_(cond), ifstmt_(ifstmt), else_stmt_(else_stmt) {};
            void exec() override;
            ~IfStmt() override;

        private:
            Ast::Expr* cond_;
            Stmt* ifstmt_;
            Stmt* else_stmt_;
    };

    class AsgmtStmt : public Stmt {
        public:
            AsgmtStmt(Ast::Expr* left, Ast::Expr* right)
                : left_(left), right_(right) {};
            void exec() override;
            ~AsgmtStmt() override;

        private:
            Ast::Expr* left_;
            Ast::Expr* right_;
    };
}

#endif

