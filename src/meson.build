# gather subsystem sources and headers into vars
# subdir('util')

error_src = files(['util/smol_error.cpp'])
error_inc = include_directories('util')

lexer_src = files(['lexer/lexer.cpp', 'lexer/token.cpp'])
lexer_inc = include_directories('lexer')

ast_inc = include_directories('ast')

parser_src = files(['parser/parser.cpp'])
parser_inc = include_directories('parser')

subsystems_src = [lexer_src, error_src, parser_src]
subsystems_inc = [lexer_inc, error_inc, ast_inc, parser_inc]