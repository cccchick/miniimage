# MiniImageRetrieval launcher for PowerShell
# This script sets PATH and QT plugin path, then starts App.exe.

$ErrorActionPreference = "Stop"

$QT_PATH        = "D:\QT\6.11.1\mingw_64\bin"
$OPENCV_PATH    = "C:\Users\chick\opencv-project\opencv-install\x64\mingw\bin"
$MINGW_PATH     = "C:\Users\chick\Desktop\mingw64\bin"
$QT_PLUGIN_PATH = "D:\QT\6.11.1\mingw_64\plugins\platforms"
$APP_DIR        = Join-Path $PSScriptRoot "build\App"
$APP_EXE        = Join-Path $APP_DIR "App.exe"
$DLL_PATH       = @(
    (Join-Path $PSScriptRoot "build\ImageLoaderDLL"),
    (Join-Path $PSScriptRoot "build\FeatureExtractorDLL"),
    (Join-Path $PSScriptRoot "build\IndexerDLL"),
    (Join-Path $PSScriptRoot "build\SearcherDLL")
) -join ";"

Write-Host "============================================"
Write-Host "MiniImageRetrieval Launcher"
Write-Host "============================================"
Write-Host ""

if (-not (Test-Path $APP_EXE)) {
    Write-Host "[ERROR] App.exe not found: $APP_EXE"
    Write-Host "[HINT] Build first with:"
    Write-Host "       cmake -S . -B build -G `"MinGW Makefiles`""
    Write-Host "       cmake --build build -j4"
    exit 1
}

if (-not (Test-Path $QT_PATH)) {
    Write-Host "[WARN] Qt path not found: $QT_PATH"
}
if (-not (Test-Path $OPENCV_PATH)) {
    Write-Host "[WARN] OpenCV path not found: $OPENCV_PATH"
}

Write-Host ""
Write-Host "[INFO] Setting environment variables..."
$env:PATH = "$MINGW_PATH;$QT_PATH;$OPENCV_PATH;$DLL_PATH;$($env:PATH)"
$env:QT_QPA_PLATFORM_PLUGIN_PATH = $QT_PLUGIN_PATH

Write-Host "[INFO] Starting MiniImageRetrieval..."
Write-Host "============================================"
Write-Host ""

Set-Location $APP_DIR
Start-Process $APP_EXE

Write-Host "[OK] Launch command sent."
Write-Host "[HINT] If the window does not open, check:"
Write-Host "       1. Qt DLLs exist in: $QT_PATH"
Write-Host "       2. OpenCV DLLs exist in: $OPENCV_PATH"
Write-Host "       3. Build succeeded without errors"
