# gather subsystem sources and headers into vars
subdir('ast')
subdir('util')
subdir('lib')

parser_src = files(['parser/parser.cpp'])
parser_inc = include_directories('parser')

lexer_src = files(['lexer/lexer.cpp', 'lexer/token.cpp'])
lexer_inc = include_directories('lexer')

util_src = files(['util/smol_error.cpp'])
util_inc = include_directories('util')

env_src = files(['env/env.cpp', 'env/fntable.cpp'])
env_inc = include_directories('env')

ffi_src = files(['lib/ffi.cpp', 'lib/userfn.cpp'])
ffi_inc = include_directories('lib')

lib_src = [ffi_src, stdlib_src]
lib_inc = [ffi_inc, stdlib_inc]
