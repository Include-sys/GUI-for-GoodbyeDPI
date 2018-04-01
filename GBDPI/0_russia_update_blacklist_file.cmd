@ECHO OFF
PUSHD "%~dp0"
bitsadmin /transfer blacklist https://antizapret.prostovpn.org/domains-export.txt "%CD%\blacklist.txt"
POPD
