#include <iostream>

#include <backtrace-supported.h>
#include <backtrace.h>

void error_callback(void *data, const char *msg, int errnum)
{
    std::cout << "error_callback" << std::endl;
}

int full_callback(void *data, uintptr_t pc, const char *filename, int lineno,
                    const char *function)
{
    std::cout << "full_callback" << std::endl;
    return 0;
}

struct backtrace_state *state;

int h()
{
    int r;
    r = backtrace_full(state, 0, full_callback, error_callback, nullptr);
    return 42;
}

int g()
{
    return h();
}

int f()
{
    return g();
}

int main()
{
    std::cout << "BACKTRACE_SUPPORTED = " << BACKTRACE_SUPPORTED << std::endl;
    std::cout << "BACKTRACE_USES_MALLOC = " << BACKTRACE_USES_MALLOC << std::endl;
    std::cout << "BACKTRACE_SUPPORTS_THREADS = " << BACKTRACE_SUPPORTS_THREADS << std::endl;
    std::cout << "BACKTRACE_SUPPORTS_DATA = " << BACKTRACE_SUPPORTS_DATA << std::endl;

    state = backtrace_create_state(nullptr, 0, error_callback, nullptr);

    int r;
    r = f();
    std::cout << r << std::endl;

    return 0;
}
