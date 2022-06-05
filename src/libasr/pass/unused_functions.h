#ifndef LFORTRAN_PASS_UNUSED_FUNCTIONS_H
#define LFORTRAN_PASS_UNUSED_FUNCTIONS_H

#include <libasr/asr.h>

namespace LCompilers {

    void pass_unused_functions(Allocator &al, ASR::TranslationUnit_t &unit);

} // namespace LCompilers

#endif // LFORTRAN_PASS_UNUSED_FUNCTIONS_H
