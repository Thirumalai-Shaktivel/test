//#define DOCTEST_CONFIG_NO_EXCEPTIONS
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
    //LFortran::print_stack_on_segfault();
#endif
    try {
        return doctest::Context(argc, argv).run();
    } catch(const LFortran::LFortranException &e) {
        std::cerr << e.stacktrace();
        std::cerr << e.name() + ": " << e.msg() << std::endl;
        return 1;
    } catch(const std::runtime_error &e) {
        std::cerr << "runtime_error: " << e.what() << std::endl;
        return 1;
    } catch(const std::exception &e) {
        std::cerr << "std::exception: " << e.what() << std::endl;
        return 1;
    } catch(...) {
        std::cerr << "Unknown Exception" << std::endl;
        return 1;
    }
}
