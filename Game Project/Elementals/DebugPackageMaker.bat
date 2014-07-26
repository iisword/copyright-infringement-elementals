XCOPY Debug\Elementals.exe DebugPackage\ /y /f
XCOPY Elementals\assets DebugPackage\assets /y /i /e /f
XCOPY ServerDebug\ElementalsServer.exe DebugPackage\ /y /f
XCOPY "Third Party\lib\*.dll" DebugPackage\ /y /i /f
PAUSE