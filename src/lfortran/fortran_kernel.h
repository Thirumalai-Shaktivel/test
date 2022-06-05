#ifndef LCOMPILERS_FORTRAN_KERNEL_H
#define LCOMPILERS_FORTRAN_KERNEL_H

#include <libasr/config.h>

namespace LCompilers {

#ifdef HAVE_LCOMPILERS_XEUS
    int run_kernel(const std::string &connection_filename);
#endif

}

#endif // LCOMPILERS_FORTRAN_KERNEL_H
