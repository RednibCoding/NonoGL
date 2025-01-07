@echo off

REM Simple build script template for small projects

echo Starting compilation...


REM @@@@@@@@@@@@@@@@@@@@@@@@@@@@ Edit the following to your liking @@@@@@@@@@@@@@@@@@@@@@@@@@@@

REM Define compiler
set COMPILER=gcc

REM Output file name
set OUTPUT_FILE_NAME=main.exe

REM Define resource files to include
set RES_FILES=

REM Define the source files to include
set SRC_FILES=main.c

REM Define additional compiler flags to use
set CFLAGS=

REM Define additional linker flags to use (add -mwindows to hide the console)
set LDFLAGS=-L../minig/internal/lib/x64 -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32

REM Set the c standard
set CSTD=-std=c99

REM Release build optimization flags
set RELEASEFLAGS=-O3 -Wall -s -fno-strict-aliasing -fomit-frame-pointer

REM Release build (comment/uncomment)
%COMPILER% -c %SRC_FILES% %CSTD% %CFLAGS% %RELEASEFLAGS% -o main.o

REM @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Edit the above to your liking @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


REM Linkage
echo Starting linking...
%COMPILER% -o %OUTPUT_FILE_NAME% main.o %LDFLAGS%

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