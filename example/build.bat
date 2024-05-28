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

REM Define additional compiler flags to use
set CFLAGS=-L../minig/lib/win64 -lglfw3 -lopengl32 -lgdi32 -luser32

REM Debug build
 %COMPILER% %SRC_FILES% %RES_FILES% -o %OUTPUT_FILE_NAME% %CFLAGS% -std=c99

REM Release build
@REM %COMPILER% %SRC_FILES% %RES_FILES% -o %OUTPUT_FILE_NAME% %CFLAGS% -O3 -Wall -s -std=c99 -fno-strict-aliasing -fomit-frame-pointer -mwindows

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