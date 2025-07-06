@echo off
setlocal

set BUILD_DIR=build
set BINARY_NAME=PhysicsEngine3D.exe

if exist "%BUILD_DIR%\%BINARY_NAME%" (
  pushd %BUILD_DIR%
  %BINARY_NAME%
  popd
) else (
  echo Binary not found. Please build the project first.
  exit /b 1
)

endlocal
