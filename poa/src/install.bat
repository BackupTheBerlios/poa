@echo off
REM Diese Batchdatei installiert POA standardmaessig nach C:\POA
REM Falls ein anderes Verzeichnis gewünscht ist, dann als erstes
REM Argument das jeweilige Verzeichnis angeben.
set PREFIX=C:\
if /I "%1" == "" goto proceed
set PREFIX=%1
:proceed
echo "POA Installation unter Windows"
echo "=============================="
echo "Installationspfad: %PREFIX%"
mkdir %PREFIX%\POA
mkdir %PREFIX%\POA\icons
mkdir %PREFIX%\POA\templates
echo "Installiere poa.exe"
xcopy poa.exe %PREFIX%\POA\
echo "Installiere Icons"
xcopy icons\* %PREFIX%\POA\icons\
echo "Installiere Templates"
xcopy templates\* %PREFIX%\POA\templates\
echo "Installation beendet."
echo "Have fun! Ihr WMI Team."
