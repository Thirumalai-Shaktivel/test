#include <lld/Common/Driver.h>

namespace LFortran {

int lld_main() {
    return lld::elf::link({"lld", "-o", "prog", "program1.o",
        "src/runtime/lfort_intrinsics.o",
        "/usr/lib/x86_64-linux-musl/crt1.o",
        "/usr/lib/x86_64-linux-musl/libc.a"
    }, false);
}

} // namespace LFortran
