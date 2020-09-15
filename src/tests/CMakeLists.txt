add_library(doctest STATIC doctest.cpp)
target_link_libraries(doctest lfortran_lib)
add_library(p::doctest ALIAS doctest)
