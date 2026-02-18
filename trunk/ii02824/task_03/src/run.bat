@echo off
setlocal enabledelayedexpansion

REM Получаем папку скрипта
set SCRIPT_DIR=%~dp0
cd /d "%SCRIPT_DIR%"

set SRC_DIR=C:\Users\User\Documents\GitHub\OTIS-2025\trunk\ii02824\task_03\src
set BUILD_DIR=%SRC_DIR%\build

echo.
echo === [1/4] Clean build directory ===
echo BUILD_DIR: %BUILD_DIR%

if exist "%BUILD_DIR%" (
rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%"

echo.
echo === [2/4] Configure (CMake) ===
cmake -S "%SRC_DIR%" -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=Debug

if errorlevel 1 (
echo ERROR: CMake configure failed
)

echo.
echo === [3/4] Build ===
cmake --build "%BUILD_DIR%" --config Debug

if errorlevel 1 (
echo ERROR: Build failed
)

echo.
echo === [4/4] Run tests ===

set TEST_EXE=%BUILD_DIR%\Debug\runTests_ii002824_task03.exe

if exist "%TEST_EXE%" (
"%TEST_EXE%"
) else (
echo ERROR: tests binary not found: %TEST_EXE%
)

echo.
echo === Run main program (pid_sim) ===

set MAIN_EXE=%BUILD_DIR%\Debug\pid_sim_ii002824.exe

if exist "%MAIN_EXE%" (
"%MAIN_EXE%"
) else (
echo ERROR: main binary not found: %MAIN_EXE%
)

start https://topg1616.github.io/OTIS-2025/

echo.
echo === DONE: build + tests + run + docs ===

pause