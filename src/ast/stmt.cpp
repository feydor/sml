#include "stmt.h"

std::string
Stmt::type_to_string(stmt_t type) {
    switch (type) {
        case VAR_DECL: return "VAR_DECL";
        case SAY_STMT: return "SAY_STMT";
        case EXPR_STMT: return "EXPR_STMT";
        default: return "Unidentifed statement.";
    }
}
