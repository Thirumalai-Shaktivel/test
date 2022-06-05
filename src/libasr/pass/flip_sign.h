#ifndef LIBASR_PASS_FLIP_SIGN_H
#define LIBASR_PASS_FLIP_SIGN_H

#include <libasr/asr.h>

namespace LCompilers {

    void pass_replace_flip_sign(Allocator &al, ASR::TranslationUnit_t &unit, const std::string& rl_path);

} // namespace LCompilers

#endif // LIBASR_PASS_FLIP_SIGN_H
