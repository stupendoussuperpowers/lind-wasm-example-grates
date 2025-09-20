# Example Grates for Lind

This reposistory contains a collection of sample grate implementations that can be used with the [Lind runtime](https://github.com/Lind-Project/lind-wasm)

Grates provide custom syscall wrappers for cages. Each example grate here overrides one or more syscalls with a custom implementation. 

For more details on Lind and grates, refer to the official [documentation.](https://lind-project.github.io/lind-wasm/)

## Repository Structure 

Each subfolder in the `examples/` directory contains a standalone grate implementation.

A typical grate consists of:

```
 - *.c  // C source files for the grate implementation
 - *.h  // Header files for exports
 - compile_grate.sh // Build script 
 - README.md // Documentation speciic to the grate
```

Currently, grates can only be implemented in C.
 
## Running a Grate

To run a cage under a grate use the helper script like so: 

`./scripts/lind_run <grate>.wasm <cage>.wasm`

This runs the cage inside lind, with the grate providing custom syscall handling.
