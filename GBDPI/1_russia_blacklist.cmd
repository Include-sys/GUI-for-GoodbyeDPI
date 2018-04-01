@ECHO OFF
PUSHD "%~dp0"
IF "%PROCESSOR_ARCHITECTURE%"=="x86" (
PUSHD "x86"
) ELSE (
PUSHD "x86_64"
)

start "" goodbyedpi.exe -1 --blacklist blacklist.txt

POPD
POPD
