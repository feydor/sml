#include "stmt.h"
using namespace Ast;

std::string
Stmt::type_to_string(Stmt_t type) {
    switch (type) {
        case Stmt_t::VAR_DECL: return "VAR_DECL";
        case Stmt_t::SAY: return "SAY_STMT";
        case Stmt_t::EXPR: return "EXPR_STMT";
        default: return "Unidentifed statement.";
    }
}
