project(
  'smol', 
  'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++17']
)

# add_project_arguments('`llvm-config --cxxflags`', language: 'cpp')

# for address sanitizer add 'cpp_link_args=-fsanitize=address,undefined' to default_options

# gather subsystem sources and headers into vars
subdir('src')

# gather main source and header
smol_src = ['src/smol.cpp']
smol_inc = include_directories('src')

# combine main with subsystems
src = [smol_src, subsystems_src]
inc = [smol_inc, subsystems_inc]

# get llvm dependency
llvm_dep = dependency('llvm', version : '>=12.0')

exe = executable(
  'smol', 
  src,
  include_directories: inc,
  install : true,
  dependencies: llvm_dep,
)

test('basic', exe)
