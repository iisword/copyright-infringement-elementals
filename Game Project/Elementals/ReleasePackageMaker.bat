XCOPY Release\Elementals.exe ReleasePackage\ /y /f
XCOPY Elementals\assets ReleasePackage\assets /y /i /e /f
XCOPY ServerRelease\ElementalsServer.exe ReleasePackage\ /y /f
PAUSE