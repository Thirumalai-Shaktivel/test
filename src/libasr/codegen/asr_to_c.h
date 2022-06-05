#ifndef LFORTRAN_ASR_TO_C_H
#define LFORTRAN_ASR_TO_C_H

#include <libasr/asr.h>

namespace LCompilers {

    Result<std::string> asr_to_c(Allocator &al, ASR::TranslationUnit_t &asr,
        diag::Diagnostics &diagnostics);

} // namespace LCompilers

#endif // LFORTRAN_ASR_TO_C_H
