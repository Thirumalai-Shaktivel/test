const fs = require('fs');

let imports = {};
// let memory = null;
// let exports = null;

let wasmBuffer = fs.readFileSync('./wasm.wasm');
// imports['memory'] = new WebAssembly['Memory']( {'initial':32} );
// memory = new Uint8Array( imports['memory']['buffer'] );

WebAssembly.instantiate(wasmBuffer, {env: imports, wasi_snapshot_preview1: exports}).then(wasmModule => {
    // Exported function live under instance.exports
    // const myadd = wasmModule.instance.exports.myadd;

    // console.log(add(4, 5));
    // console.log(myadd(4, 5));
    const add = wasmModule.instance.exports.add;
    console.log(add(5, 4))
    console.log("Success!")
});
