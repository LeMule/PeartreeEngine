@echo off
setlocal
cd /d %~dp0

echo Select your desired visual studio:
echo 1) Visual Studio 2019 64-bit
REM echo 2) Visual Studio 2010 64-bit (Unsupported, see issues list)
REM echo 3) Visual Studio 2010 32-bit (Unsupported, see issues list)

echo.
set /p SELECTED_OPTION=
if "%SELECTED_OPTION%"=="1" (
	goto :SETUP_VS2019_x64
REM ) else if "%SELECTED_OPTION%"=="2" (
	REM goto :SETUP_VS2010_x64
REM ) else if "%SELECTED_OPTION%"=="3" (
	REM goto :SETUP_VS2010
) else (
	echo Restart and Select a valid option.
	pause
	exit /b 1
)

:SETUP_VS2019_x64
	set CMAKE_GENERATOR=-G"Visual Studio 16 2019"
	set BUILD_SUFFIX=_2019_x64
	goto :GENERATE
:SETUP_VS2010_x64
	set CMAKE_GENERATOR=-G"Visual Studio 10 2010 Win64"
	set BUILD_SUFFIX=_2010_x64
	goto :GENERATE
:SETUP_VS2010
	set CMAKE_GENERATOR=-G"Visual Studio 10 2010"
	set BUILD_SUFFIX=_2010
	goto :GENERATE

:GENERATE
set BUILD_FOLDER="build_win32%BUILD_SUFFIX%"
mkdir %BUILD_FOLDER% > NUL 2>&1
pushd %BUILD_FOLDER%

cmake -Wno-dev %CMAKE_GENERATOR% ../Core
popd
endlocal
pause