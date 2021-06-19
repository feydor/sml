# gather ast sources and headers into vars
ast_expr_src = files(
  ['expr/literal.cpp', 'expr/asgmt.cpp',
    'expr/binary.cpp', 'expr/unary.cpp', 'expr/var.cpp',
    'expr/fn.cpp', 'expr/cond.cpp']
)
ast_expr_inc = include_directories('expr')

ast_stmt_src = files(['stmt/asgmtstmt.cpp', 'stmt/blockstmt.cpp',
  'stmt/ifstmt.cpp', 'stmt/saystmt.cpp', 'stmt/exprstmt.cpp'])
ast_stmt_inc = include_directories('stmt')

ast_src = [ast_expr_src, ast_stmt_src]
ast_inc = [ast_expr_inc, ast_stmt_inc]
