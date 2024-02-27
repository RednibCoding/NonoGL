@echo off

echo Starting compilation...

REM Define resource files to include
set RES_FILES=

REM Define the source files to include
set SRC_FILES= test.c

REM Debug build
 gcc %SRC_FILES% %RES_FILES% -o test.exe -std=c99 -lopengl32 -lgdi32

REM Release build
@REM gcc %SRC_FILES% %RES_FILES% -o test.exe -std=c99 -s -lopengl32 -lgdi32 -O3 -march=native -funroll-loops -flto -fomit-frame-pointer -mwindows


if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    exit /b %ERRORLEVEL%
)
echo Compilation succeeded.
test.exe
