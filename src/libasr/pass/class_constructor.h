#ifndef LFORTRAN_PASS_CLASS_CONSTRUCTOR_H
#define LFORTRAN_PASS_CLASS_CONSTRUCTOR_H

#include <libasr/asr.h>

namespace LCompilers {

    void pass_replace_class_constructor(Allocator &al, ASR::TranslationUnit_t &unit);

} // namespace LCompilers

#endif // LFORTRAN_PASS_CLASS_CONSTRUCTOR_H
