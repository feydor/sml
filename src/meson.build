# gather subsystem sources and headers into vars
subdir('ast')

parser_src = files(['parser/parser.cpp'])
parser_inc = include_directories('parser')

lexer_src = files(['lexer/lexer.cpp', 'lexer/token.cpp'])
lexer_inc = include_directories('lexer')

util_src = files(['util/value.cpp', 'util/smol_error.cpp'])
util_inc = include_directories('util')

env_src = files(['env/env.cpp', 'env/fntable.cpp', 'env/retstack.cpp'])
env_inc = include_directories('env')

lib_src = files(['lib/ffi.cpp', 'lib/userfn.cpp', 'lib/tostr.cpp'])
lib_inc = include_directories('lib')
