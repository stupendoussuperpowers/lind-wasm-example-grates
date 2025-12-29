See `src/imfs_grate.c` to see an example of how to register a syscall handler.

See `src/tmp.c` to see the codegen output based on the above file.

## Compiling

Use the `gratec` script in the `lind-wasm-example-grates/tools` folder. 


`./gratec <path/to/grate/folder>`

This generates `.wasm` and `.cwasm` output binaries in the `output/` folder.

