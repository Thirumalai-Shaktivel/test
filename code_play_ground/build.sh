$HOME/wasi-sdk-12.0/bin/wasm-ld \
    --no-entry \
    wasm_cosh.o \
    lfruntime_intrinsics_c_wasm32.o \
    -L$HOME/wasi-sdk-12.0/lib/clang/11.0.0/lib/wasi \
    -L$HOME/wasi-sdk-12.0/share/wasi-sysroot/lib/wasm32-wasi \
    -lclang_rt.builtins-wasm32 \
    -lc \
    --export-dynamic \
    --strip-all \
    -o wasm_cosh.wasm
