#define DOCTEST_CONFIG_NO_POSIX_SIGNALS
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <lfortran/config.h>
#include <lfortran/stacktrace.h>

int main(int argc, char** argv)
{
#if defined(HAVE_LFORTRAN_STACKTRACE)
    LFortran::print_stack_on_segfault();
#endif
    return doctest::Context(argc, argv).run();
}
