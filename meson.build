project(
  'smol', 
  'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++17']
)

inc = include_directories('inc')
src = ['src/smol.cpp', 'src/lexer.cpp', 'src/token.cpp', 'src/parser.cpp', 'src/expr.cpp']
exe = executable(
  'smol', 
  src,
  include_directories: inc,
  install : true
)

gen_ast = executable(
  'gen_ast',
  ['util/generate_ast.cpp'],
  install: false
)

test('basic', exe)
