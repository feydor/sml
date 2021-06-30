project(
  'smol', 
  'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++17']
)

# for address sanitizer add 'cpp_link_args=-fsanitize=address,undefined' to default_options

# gather subsystem sources and headers into vars
subdir('src')

# gather main source and header
smol_src = ['src/smol.cpp']
smol_inc = include_directories('src')

# combine main with subsystems
src = [smol_src, ast_src, env_src, lib_src,
      parser_src, lexer_src, util_src, object_src]
inc = [smol_inc, ast_inc, env_inc, lib_inc,
      parser_inc, lexer_inc, util_inc, object_inc]

exe = executable(
  'smol', 
  src,
  include_directories: inc,
  install : true
)

test('basic', exe)
