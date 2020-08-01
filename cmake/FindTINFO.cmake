find_library(TINFO_LIBRARY tinfo)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TINFO DEFAULT_MSG TINFO_LIBRARY)

add_library(p::tinfo INTERFACE IMPORTED)
set_property(TARGET p::tinfo PROPERTY INTERFACE_LINK_LIBRARIES
    ${TINFO_LIBRARY})
