# gather subsystem sources and headers into vars
# subdir('util')

error_src = files(['util/smol_error.cpp'])
error_inc = include_directories('util')

lexer_src = files(['lexer/lexer.cpp', 'lexer/token.cpp'])
lexer_inc = include_directories('lexer')

subsystems_src = [lexer_src, error_src]
subsystems_inc = [lexer_inc, error_inc]