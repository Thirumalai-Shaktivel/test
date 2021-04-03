#ifndef LFORTRAN_PASS_nested_vars_H
#define LFORTRAN_PASS_nested_vars_H

#include <lfortran/asr.h>

namespace LFortran {

    std::vector<uint64_t> pass_find_nested_vars(Allocator &al, 
            ASR::TranslationUnit_t &unit);

} // namespace LFortran

#endif // LFORTRAN_PASS_nested_vars_H
