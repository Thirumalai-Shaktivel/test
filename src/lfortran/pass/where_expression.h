#ifndef LFORTRAN_PASS_WHERE_EXPRESSION_H
#define LFORTRAN_PASS_WHERE_EXPRESSION_H

#include <lfortran/asr.h>

namespace LFortran {

    void pass_replace_where_expression(Allocator &al, ASR::TranslationUnit_t &unit);

} // namespace LFortran

#endif // LFORTRAN_PASS_WHERE_EXPRESSION_H
