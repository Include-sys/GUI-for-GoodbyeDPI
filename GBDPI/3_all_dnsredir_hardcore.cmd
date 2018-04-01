@ECHO OFF
PUSHD "%~dp0"
IF "%PROCESSOR_ARCHITECTURE%"=="x86" (
PUSHD "x86"
) ELSE (
PUSHD "x86_64"
)

start "" goodbyedpi.exe -1 -a -m --dns-addr 77.88.8.8 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253

POPD
POPD
