#ifndef LFORTRAN_PASS_FLIP_SIGN_H
#define LFORTRAN_PASS_FLIP_SIGN_H

#include <lfortran/asr.h>

namespace LFortran {

    void pass_replace_flip_sign(Allocator &al, ASR::TranslationUnit_t &unit);

} // namespace LFortran

#endif // LFORTRAN_PASS_FLIP_SIGN_H
