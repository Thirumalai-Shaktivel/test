#ifndef LFORTRAN_MOD_TO_ASR_H
#define LFORTRAN_MOD_TO_ASR_H

#include <libasr/asr.h>

namespace LCompilers {

    ASR::TranslationUnit_t *mod_to_asr(Allocator &al, std::string filename);

} // namespace LCompilers

#endif // LFORTRAN_MOD_TO_ASR_H
