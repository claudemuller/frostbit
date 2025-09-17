@echo off
set "CompilerFlags=-Wall -Wextra -std=c99 -Wno-microsoft -Wpre-c11-compat -Wno-pragma-pack -I^"C:\lib\SDL3-3.2.22\include^""

set "LinkerFlags=/LIBPATH:^"C:\lib\SDL3-3.2.22\lib\x64^" SDL3.lib /DEBUG /OUT:game.exe"

if not exist bin mkdir bin
pushd bin

del *.pdb >NUL 2>&1
del *.obj >NUL 2>&1
del *.exe >NUL 2>&1

clang-cl.exe %CompilerFlags% ..\src\main.c ..\src\game.c ..\src\system.c ..\src\texture.c ..\src\entity.c ..\src\arena.c -c 
if errorlevel 1 (
    echo *** Compilation failed ***
    popd & exit /b 1
)

clang-cl.exe main.obj game.obj system.obj entity.obj texture.obj arena.obj /link %LinkerFlags%
if errorlevel 1 (
    echo *** Linking failed ***
) else (
    echo *** Build succeeded: bin\sdl3.exe ***
)

popd
