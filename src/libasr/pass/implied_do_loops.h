#ifndef LFORTRAN_PASS_IMPLIED_DO_LOOPS_H
#define LFORTRAN_PASS_IMPLIED_DO_LOOPS_H

#include <libasr/asr.h>

namespace LCompilers {

    void pass_replace_implied_do_loops(Allocator &al, ASR::TranslationUnit_t &unit,
        const std::string &rl_path);

} // namespace LCompilers

#endif // LFORTRAN_PASS_IMPLIED_DO_LOOPS_H
