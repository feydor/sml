project(
  'smol', 
  'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++17']
)

inc = include_directories('inc')
src = [
  'src/smol.cpp', 'src/lexer.cpp', 'src/token.cpp', 'src/parser.cpp',
  'src/expr.cpp', 'src/intpr.cpp', 'src/stmt.cpp', 'src/sym.cpp',
]
exe = executable(
  'smol', 
  src,
  include_directories: inc,
  install : true
)

test('basic', exe)
