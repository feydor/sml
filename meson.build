project(
  'sml', 
  'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++14']
)

inc = include_directories('inc')
src = ['src/sml.cpp', 'src/lexer.cpp']
exe = executable(
  'sml', 
  src,
  include_directories: inc,
  install : true
)

test('basic', exe)
