@echo off

REM Simple build script template for small projects
REM No need for 'make' and stuff

echo Starting compilation...

REM Define compiler
set COMPILER=gcc

REM Output file name
set OUTPUT_FILE_NAME=main.exe

REM Define resource files to include
set RES_FILES=

REM Define the source files to include
set SRC_FILES=main.c

REM Define additional flags to use
set FLAGS=-D FREEGLUT_STATIC -L../minig/lib/win32/x64 -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32

REM Debug build
 %COMPILER% %SRC_FILES% %RES_FILES% -o %OUTPUT_FILE_NAME% %FLAGS% -std=c99

REM Release build
@REM %COMPILER% %SRC_FILES% %RES_FILES% -o %OUTPUT_FILE_NAME% %FLAGS% -O3 -Wall -s -std=c99 -fno-strict-aliasing -fomit-frame-pointer -mwindows

if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    exit /b %ERRORLEVEL%
)
echo Compilation succeeded.

REM Check if the output file exists before attempting to run it
if exist %OUTPUT_FILE_NAME% (
    echo Running %OUTPUT_FILE_NAME%...
    %OUTPUT_FILE_NAME%
) else (
    echo %OUTPUT_FILE_NAME% not found.
)