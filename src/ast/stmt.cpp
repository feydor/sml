#include "stmt.h"
using namespace Ast;

Ident *
IdentStmt::ident()
{
    return this->_ident;
}
        
Expr *
Stmt::expr()
{
    return this->_expr;
}

bool
Stmt::is_say_stmt()
{
    return this->type == Stmt_t::SAY;
}

bool
Stmt::is_expr_stmt()
{
    return this->type == Stmt_t::EXPR;
}

bool
Stmt::is_decl_stmt()
{
    return this->type == Stmt_t::VAR_DECL;
}

bool
Stmt::is_def_stmt()
{
    return this->type == Stmt_t::VAR_DEF;
}

std::string
Stmt::type_to_string(Stmt const *stmt) {
    switch (stmt->type) {
        case Stmt_t::VAR_DECL: return "VAR_DECL";
        case Stmt_t::VAR_DEF: return "VAR_DEF";
        case Stmt_t::SAY: return "SAY_STMT";
        case Stmt_t::EXPR: return "EXPR_STMT";
        default: return "Unidentifed statement.";
    }
}
