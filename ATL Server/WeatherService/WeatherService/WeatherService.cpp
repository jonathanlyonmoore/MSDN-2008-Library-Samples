// WeatherService.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
// For custom assert and trace handling with WebDbg.exe
#ifdef _DEBUG
CDebugReportHook g_ReportHook;
#endif

#include "WeatherService.h"
[ module(name="MyWeatherService", type=dll) ]
class CDllMainOverride
{
public:

};

[ emitidl(restricted) ];
