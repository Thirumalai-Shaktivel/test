find_path(RANG_HEADER rang.hpp PATHS src/tpl)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RANG DEFAULT_MSG RANG_HEADER)

add_library(p::rang INTERFACE IMPORTED)
set_property(TARGET p::rang PROPERTY INTERFACE_INCLUDE_DIRECTORIES
    ${RANG_HEADER})
