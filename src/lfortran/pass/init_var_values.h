#ifndef LFORTRAN_PASS_INIT_VAR_VALUES_H
#define LFORTRAN_PASS_INIT_VAR_VALUES_H

#include <lfortran/asr.h>

namespace LFortran {

    void pass_replace_init_var_values(Allocator &al, ASR::TranslationUnit_t &unit);

} // namespace LFortran

#endif // LFORTRAN_PASS_INIT_VAR_VALUES_H
