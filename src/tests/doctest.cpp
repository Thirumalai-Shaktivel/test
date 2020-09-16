#define DOCTEST_CONFIG_NO_EXCEPTIONS
#define DOCTEST_CONFIG_NO_POSIX_SIGNALS
#define DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS
#define DOCTEST_CONFIG_IMPLEMENT
#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#include "doctest.h"

#include <lfortran/config.h>
#include <lfortran/stacktrace.h>
#include <lfortran/exception.h>

int main(int argc, char** argv)
{
#if defined(HAVE_LFORTRAN_STACKTRACE)
    LFortran::print_stack_on_segfault();
#endif
    return doctest::Context(argc, argv).run();
}
