# 🧊 Frostbit

[![c](https://github.com/claudemuller/frostbit/actions/workflows/c.yml/badge.svg)](https://github.com/claudemuller/frostbit/actions/workflows/c.yml)

A game engine written in C with SDL.

## Requirements

- [gcc](https://gcc.gnu.org/)
- or [clang](https://clang.llvm.org/)
- [make](https://www.gnu.org/software/make/)
- [SDL2](https://www.libsdl.org/)
- (Optional) [Bear](https://github.com/rizsotto/Bear) - for auto-completion (in the editor)

## Generate Compilation Database (for auto-completion)

```bash
make gen-compilation-db
```

## Building

```bash
make build
```

## Running

```bash
make run
```

## Build a Debug Binary

```bash
make debug-build
```

## Start `lldb` or `gdb` With Debug Binary

```bash
make debug
```

## Cleaning the Project

```bash
make clean
```
