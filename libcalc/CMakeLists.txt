add_library(libcalc lex.cpp parse.cpp)
target_compile_options(libcalc PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(libcalc fmt::fmt)

