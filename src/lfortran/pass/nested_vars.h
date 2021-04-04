#ifndef LFORTRAN_PASS_NESTED_VARS_H
#define LFORTRAN_PASS_NESTED_VARS_H

#include <lfortran/asr.h>

namespace LFortran {

    std::vector<uint64_t> pass_find_nested_vars(ASR::TranslationUnit_t &unit);

} // namespace LFortran

#endif // LFORTRAN_PASS_NESTED_VARS_H
