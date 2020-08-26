find_path(BACKTRACE_INCLUDE_DIR backtrace.h)
find_library(BACKTRACE_LIBRARY libbacktrace.a)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BACKTRACE DEFAULT_MSG BACKTRACE_INCLUDE_DIR BACKTRACE_LIBRARY)

add_library(p::backtrace INTERFACE IMPORTED)
set_property(TARGET p::backtrace PROPERTY INTERFACE_INCLUDE_DIRECTORIES
    ${BACKTRACE_INCLUDE_DIR})
set_property(TARGET p::backtrace PROPERTY INTERFACE_LINK_LIBRARIES
    ${BACKTRACE_LIBRARY})
