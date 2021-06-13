project(
  'smol', 
  'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++17']
)

# gather subsystem sources and headers into vars
subdir('src')

# gather main source and header
smol_src = ['src/smol.cpp']
smol_inc = include_directories('src')

# combine main with subsystems
src = [smol_src, ast_expr_src, ast_stmt_src, intpr_src, parser_src, lexer_src, val_src]
inc = [smol_inc, ast_expr_inc, ast_stmt_inc, intpr_inc, parser_inc, lexer_inc, val_inc]

exe = executable(
  'smol', 
  src,
  include_directories: inc,
  install : true
)

test('basic', exe)
