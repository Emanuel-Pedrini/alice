@echo off
setlocal
cd /d "%~dp0"

llvm-config --cflags --ldflags --libs --system-libs > llvm_flags.txt
gcc -IAlicec Alicec/Source/*.c @llvm_flags.txt -o Alice.exe

if errorlevel 1 (
    echo Failed to compile alice!
    exit /b 1
)

"%~dp0Alice.exe" %*
echo %errorlevel%
