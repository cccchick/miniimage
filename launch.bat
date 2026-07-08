@echo off
chcp 65001 >nul
echo ============================================
echo MiniImageRetrieval Launcher
echo ============================================
echo.

set "QT_PATH=D:\QT\6.11.1\mingw_64\bin"
set "OPENCV_PATH=C:\Users\chick\opencv-project\opencv-install\x64\mingw\bin"
set "MINGW_PATH=C:\Users\chick\Desktop\mingw64\bin"
set "APP_DIR=%~dp0build\App"
set "APP_EXE=%APP_DIR%\App.exe"
set "QT_PLUGIN_PATH=D:\QT\6.11.1\mingw_64\plugins\platforms"
set "DLL_PATH=%~dp0build\ImageLoaderDLL;%~dp0build\FeatureExtractorDLL;%~dp0build\IndexerDLL;%~dp0build\SearcherDLL"

echo [INFO] App path: %APP_EXE%
if not exist "%APP_EXE%" (
    echo [ERROR] App.exe not found. Please build first.
    echo [HINT] Run: cmake -S . -B build -G "MinGW Makefiles" ^&^& cmake --build build -j4
    pause
    exit /b 1
)

if not exist "%QT_PATH%" (
    echo [WARN] Qt path not found: %QT_PATH%
)
if not exist "%OPENCV_PATH%" (
    echo [WARN] OpenCV path not found: %OPENCV_PATH%
)

echo.
echo [INFO] Setting environment variables...
set "PATH=%MINGW_PATH%;%QT_PATH%;%OPENCV_PATH%;%DLL_PATH%;%PATH%"
set "QT_QPA_PLATFORM_PLUGIN_PATH=%QT_PLUGIN_PATH%"

echo [INFO] Starting MiniImageRetrieval...
echo ============================================
echo.

cd /d "%APP_DIR%"
start "" "%APP_EXE%"

echo [OK] Launch command sent.
echo [HINT] If the window does not open, check:
echo        1. Qt DLLs exist in: %QT_PATH%
echo        2. OpenCV DLLs exist in: %OPENCV_PATH%
echo        3. Build succeeded without errors
echo.
pause
