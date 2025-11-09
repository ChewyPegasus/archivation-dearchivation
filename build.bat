@echo off
setlocal

:: --- Configuration ---
set "PROJECT_NAME=arch_dearch"
set "SRC_DIR=src"
set "ARCHIVER_DIR=%SRC_DIR%\archiver"
set "BUILD_DIR=build"

:: --- Script Logic ---
echo .
echo [i] Building '%PROJECT_NAME%' project...
echo [i] Project structure:
echo    %SRC_DIR%\
echo    +-- main.cpp
echo    \-- %ARCHIVER_DIR%\
echo        +-- archiver.h
echo        \-- archiver.cpp
echo .

:: Check if essential source files exist
if not exist "%SRC_DIR%\main.cpp" (
    echo [!] Error: Source files not found in the expected directory structure.
    goto :error
)

:: Clean previous build directory
if exist "%BUILD_DIR%" (
    echo [i] Cleaning previous build...
    rmdir /s /q "%BUILD_DIR%"
)

:: Create and enter build directory
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

:: Configure with CMake
echo [i] Configuring CMake...
cmake ..
if %errorlevel% neq 0 (
    echo [!] CMake configuration failed
    goto :error
)

:: Build the project using all available processors
echo [i] Building with %NUMBER_OF_PROCESSORS% cores...
cmake --build . --config Release --parallel %NUMBER_OF_PROCESSORS%
if %errorlevel% neq 0 (
    echo [!] Build failed
    goto :error
)

echo [+] Build successful!
echo [i] Executable: %cd%\Release\%PROJECT_NAME%.exe

:: Return to root project directory
cd ..

:: Create a sample test file if it doesn't exist
if not exist "input.txt" (
    echo [i] Creating sample 'input.txt' file...
    (
        echo AAAAAABBBBBCCCCCDDDDD
        echo Hello World!!!
        echo 1111122222333334444455555
        echo This is a test line with no repeats.
        echo AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    ) > input.txt
)

echo .
echo [>] To run the program:
echo    %BUILD_DIR%\Release\%PROJECT_NAME%.exe
echo .
echo [i] Files will be:
echo    Input:      input.txt
echo    Compressed: archived.txt
echo    Output:     output.txt
echo .
pause
goto :eof

:error
echo .
echo Build process stopped due to an error.
pause
