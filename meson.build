project(
  'sml', 
  'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14']
)

inc = include_directories('inc')
exe = executable(
  'sml', 
  'src/sml.cpp',
  include_directories: inc,
  install : true
)

test('basic', exe)
