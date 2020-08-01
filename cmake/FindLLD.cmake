find_package(LLVM REQUIRED)

find_path(LLD_INCLUDE_DIR lld/Common/Driver.h HINTS ${LLVM_INCLUDE_DIR})
find_library(LLD_LIBRARY_ELF lldELF HINTS ${LLVM_LIBRARY_DIR})
find_library(LLD_LIBRARY_COMMON lldCommon HINTS ${LLVM_LIBRARY_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LLD DEFAULT_MSG LLD_INCLUDE_DIR
    LLD_LIBRARY_ELF LLD_LIBRARY_COMMON)

add_library(p::lld INTERFACE IMPORTED)
set_property(TARGET p::lld PROPERTY INTERFACE_INCLUDE_DIRECTORIES
    ${LLD_INCLUDE_DIR})
set_property(TARGET p::lld PROPERTY INTERFACE_COMPILE_OPTIONS
    $<$<COMPILE_LANGUAGE:CXX>:${LFORTRAN_CXX_NO_RTTI_FLAG}>)
set_property(TARGET p::lld PROPERTY INTERFACE_LINK_LIBRARIES
    ${LLD_LIBRARY_ELF} ${LLD_LIBRARY_COMMON})
target_link_libraries(p::lld INTERFACE p::llvm)
