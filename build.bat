@echo off
chcp 65001 >nul
echo ============================================
echo MiniImageRetrieval Build Script
echo ============================================
echo.

set "BUILD_DIR=%~dp0build"
set "CMAKE_PATH=cmake"
set "MAKE_PATH=mingw32-make"

echo [INFO] Checking CMake...
where %CMAKE_PATH% >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found. Please add it to PATH.
    pause
    exit /b 1
)

echo [INFO] Checking MinGW make...
where %MAKE_PATH% >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] mingw32-make not found. Please add MinGW to PATH.
    pause
    exit /b 1
)

echo.
echo [STEP 1] Creating build directory...
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
    if %errorlevel% neq 0 (
        echo [ERROR] Failed to create build directory.
        pause
        exit /b 1
    )
    echo [OK] Build directory created.
) else (
    echo [INFO] Build directory exists, reconfiguring.
)

echo.
echo [STEP 2] Running CMake configure...
cd /d "%BUILD_DIR%"
%CMAKE_PATH% .. -G "MinGW Makefiles"
if %errorlevel% neq 0 (
    echo [ERROR] CMake configure failed.
    pause
    exit /b 1
)
echo [OK] CMake configure done.

echo.
echo [STEP 3] Building project...
%MAKE_PATH%
if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)
echo [OK] Build finished.

echo.
echo ============================================
echo Build succeeded!
echo ============================================
echo.
echo App location: %BUILD_DIR%\App\App.exe
echo Run with: launch.bat or launch.ps1
echo.
pause
