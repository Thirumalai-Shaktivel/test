import { readFile } from 'node:fs/promises';
import { WASI } from 'wasi';
import { argv, env } from 'node:process';

const wasi = new WASI({
  args: argv,
  env,
  preopens: {
    '/sandbox': '/home/ubaid/Desktop/My-Projects/GSoC/CodePlayGround'
  }
});

// Some WASI binaries require:
//   const importObject = { wasi_unstable: wasi.wasiImport };
const importObject = { wasi_snapshot_preview1: wasi.wasiImport };

const wasm = await WebAssembly.compile(
  await readFile(new URL('./final.wasm', import.meta.url))
);
const instance = await WebAssembly.instantiate(wasm, importObject);

wasi.start(instance);
