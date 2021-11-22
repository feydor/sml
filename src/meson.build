# gather subsystem sources and headers into vars
# subdir('util')

error_src = files(['util/smol_error.cpp'])
error_inc = include_directories('util')

lexer_src = files(['lexer/lexer.cpp', 'lexer/token.cpp'])
lexer_inc = include_directories('lexer')

ast_src = files(['ast/function.cpp', 'ast/expr.cpp'])
ast_inc = include_directories('ast')

parser_src = files(['parser/parser.cpp'])
parser_inc = include_directories('parser')

jit_src = files(['jit/jit.hpp'])
jit_inc = include_directories('jit')

subsystems_src = [lexer_src, error_src, ast_src, parser_src, jit_src]
subsystems_inc = [lexer_inc, error_inc, ast_inc, parser_inc, jit_inc]