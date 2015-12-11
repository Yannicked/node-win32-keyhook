@echo off

reg.exe query "HKLM\SOFTWARE\Microsoft\MSBuild\ToolsVersions\4.0" /v MSBuildToolsPath > nul 2>&1
if ERRORLEVEL 1 goto MissingMSBuildRegistry

for /f "skip=2 tokens=2,*" %%A in ('reg.exe query "HKLM\SOFTWARE\Microsoft\MSBuild\ToolsVersions\4.0" /v MSBuildToolsPath') do SET MSBUILDDIR=%%B

IF NOT EXIST %MSBUILDDIR%nul goto MissingMSBuildToolsPath
IF NOT EXIST %MSBUILDDIR%msbuild.exe goto MissingMSBuildExe

"%MSBUILDDIR%msbuild.exe" "%cd%\src\GlobalKeypress\GlobalKeypress.vcxproj" /p:configuration=release /p:platform=x64
"%MSBUILDDIR%msbuild.exe" "%cd%\src\GlobalKeypress\GlobalKeypress.vcxproj" /p:configuration=release /p:platform=win32
mkdir "%cd%\bin\"
move "%cd%\src\GlobalKeypress\x64\release\GlobalKeypress.dll" "%cd%\bin\keyhook.x64.dll"
move "%cd%\src\GlobalKeypress\release\GlobalKeypress.dll" "%cd%\bin\keyhook.ia32.dll"
rmdir /s/q "%cd%\src\GlobalKeypress\x64\"
rmdir /s/q "%cd%\src\GlobalKeypress\release\"
exit /b 0

:MissingMSBuildRegistry
echo Cannot obtain path to MSBuild tools from registry
exit /b 1
:MissingMSBuildToolsPath
echo The MSBuild tools path from the registry '%MSBUILDDIR%' does not exist
exit /b 1
:MissingMSBuildExe
echo The MSBuild executable could not be found at '%MSBUILDDIR%'
exit /b 1