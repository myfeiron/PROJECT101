@echo off
echo Building SVG Processor with GUI support...

set CC=gcc
set CFLAGS=-Wall -Wextra -std=c99 -O2 -Iinclude "-Igui_libs\SDL2-2.30.6\include"
set LDFLAGS="-Lgui_libs\SDL2-2.30.6\lib\x64" -lSDL2 -lm
set SOURCES=src/main.c src/svg_parser.c src/svg_render.c src/bmp_writer.c src/svg_gui.c src/svg_writer.c
set OUTPUT=build/svg_processor.exe

echo Compiling...
%CC% %CFLAGS% %SOURCES% -o %OUTPUT% %LDFLAGS%

if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    echo Copying SDL2.dll to build directory...
    copy gui_libs\SDL2-2.30.6\lib\x64\SDL2.dll build\ >nul 2>&1
    echo Done!
) else (
    echo Build failed!
    exit /b 1
)
