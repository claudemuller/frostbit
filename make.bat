REM @echo off
setlocal

set CompilerFlags=-target x86_64-pc-windows-gnu -Wall -Wextra -std=c11 -g --sysroot="C:/msys64/mingw64" -I"C:\lib\SDL3-3.2.22\include" -I"C:\lib\SDL3_image-3.2.4\include" -I"C:\lib\tmx-1.10.0\include" -I"C:\lib\tmx-1.10.0\include\libxml2\libxml" -I"lib"
set LinkerFlags=-L"C:\lib\SDL3-3.2.22\lib\x64" -L"C:\lib\SDL3_image-3.2.4\lib\x64" -L"C:\lib\tmx-1.10.0\lib" -lSDL3 -lSDL3_image -ltmx -lxml2 -lzlibstatic

if not exist bin mkdir bin

REM del *.pdb >NUL 2>&1
REM del *.obj >NUL 2>&1
REM del *.exe >NUL 2>&1

clang %CompilerFlags% src\*.c src\utils\*.c -o bin\frostbit.exe %LinkerFlags%

endlocal
