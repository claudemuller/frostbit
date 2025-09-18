# 🧊 Frostbit

[![c](https://github.com/claudemuller/frostbit/actions/workflows/c.yml/badge.svg)](https://github.com/claudemuller/frostbit/actions/workflows/c.yml)

A game engine written in C with SDL.

## Features

- Entity Component System
- Event Bus

## Requirements

- [gcc](https://gcc.gnu.org/)
- or [clang](https://clang.llvm.org/)
- [make](https://www.gnu.org/software/make/)
- [SDL3](https://www.libsdl.org/)
- (Optional) [Bear](https://github.com/rizsotto/Bear) - for auto-completion (in the editor)

## Building

```bash
# in *nix
make build

# in doze
build.bat
```

## Running

```bash
# in *nix
make run

# in doze
bin\frostbit.exe
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

## Generate Compilation Database (for auto-completion)

```bash
make gen-compilation-db
```
