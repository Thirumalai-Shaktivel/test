#ifndef LFORTRAN_PASS_WHERE_TO_LOOP_H
#define LFORTRAN_PASS_WHERE_TO_LOOP_H

#include <lfortran/asr.h>

namespace LFortran {

    void pass_replace_where(Allocator &al, ASR::TranslationUnit_t &unit);

} // namespace LFortran

#endif // LFORTRAN_PASS_WHERE_TO_LOOP_H
