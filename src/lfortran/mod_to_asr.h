#ifndef LCOMPILERS_MOD_TO_ASR_H
#define LCOMPILERS_MOD_TO_ASR_H

#include <libasr/asr.h>

namespace LCompilers {

    ASR::TranslationUnit_t *mod_to_asr(Allocator &al, std::string filename);

} // namespace LCompilers

#endif // LCOMPILERS_MOD_TO_ASR_H
