# Microsoft Developer Studio Generated NMAKE File, Based on ATLGdiWorld.dsp
!IF "$(CFG)" == ""
CFG=ATLGdiWorld - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ATLGdiWorld - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ATLGdiWorld - Win32 Debug" && "$(CFG)" != "ATLGdiWorld - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ATLGdiWorld.mak" CFG="ATLGdiWorld - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ATLGdiWorld - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ATLGdiWorld - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ATLGdiWorld - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ATLGdiWorld.dll" "$(OUTDIR)\ATLGdiWorld.bsc" ".\Debug\regsvr32.trg"

!ELSE 

ALL : "ATLTangramCanvas - Win32 Debug" "$(OUTDIR)\ATLGdiWorld.dll" "$(OUTDIR)\ATLGdiWorld.bsc" ".\Debug\regsvr32.trg"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ATLTangramCanvas - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ATLGdiWorld.obj"
	-@erase "$(INTDIR)\ATLGdiWorld.pch"
	-@erase "$(INTDIR)\ATLGdiWorld.res"
	-@erase "$(INTDIR)\ATLGdiWorld.sbr"
	-@erase "$(INTDIR)\AtlGdiWorldImpl.obj"
	-@erase "$(INTDIR)\AtlGdiWorldImpl.sbr"
	-@erase "$(INTDIR)\AtlTangramGdiVisual.obj"
	-@erase "$(INTDIR)\AtlTangramGdiVisual.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ATLGdiWorld.bsc"
	-@erase "$(OUTDIR)\ATLGdiWorld.dll"
	-@erase "$(OUTDIR)\ATLGdiWorld.exp"
	-@erase "$(OUTDIR)\ATLGdiWorld.ilk"
	-@erase "$(OUTDIR)\ATLGdiWorld.lib"
	-@erase "$(OUTDIR)\ATLGdiWorld.pdb"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\ATLModel" /I "..\ATLTangramCanvas" /I "..\\" /D "_DEBUG" /D "_USRDLL" /D "WIN32" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ATLGdiWorld.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ATLGdiWorld.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ATLGdiWorld.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ATLGdiWorld.sbr" \
	"$(INTDIR)\AtlGdiWorldImpl.sbr" \
	"$(INTDIR)\AtlTangramGdiVisual.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\ATLGdiWorld.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib glaux.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ATLGdiWorld.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ATLGdiWorld.dll" /implib:"$(OUTDIR)\ATLGdiWorld.lib" /pdbtype:sept /idlout:_AtlGdiWorld.idl 
LINK32_OBJS= \
	"$(INTDIR)\ATLGdiWorld.obj" \
	"$(INTDIR)\AtlGdiWorldImpl.obj" \
	"$(INTDIR)\AtlTangramGdiVisual.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ATLGdiWorld.res" \
	"..\ATLTangramCanvas\Debug\ATLTangramCanvas.lib"

"$(OUTDIR)\ATLGdiWorld.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\ATLGdiWorld.dll
InputPath=.\Debug\ATLGdiWorld.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "ATLGdiWorld - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ATLGdiWorld.dll" ".\Release\regsvr32.trg"

!ELSE 

ALL : "ATLTangramCanvas - Win32 Release" "$(OUTDIR)\ATLGdiWorld.dll" ".\Release\regsvr32.trg"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ATLTangramCanvas - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ATLGdiWorld.obj"
	-@erase "$(INTDIR)\ATLGdiWorld.pch"
	-@erase "$(INTDIR)\ATLGdiWorld.res"
	-@erase "$(INTDIR)\AtlGdiWorldImpl.obj"
	-@erase "$(INTDIR)\AtlTangramGdiVisual.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ATLGdiWorld.dll"
	-@erase "$(OUTDIR)\ATLGdiWorld.exp"
	-@erase "$(OUTDIR)\ATLGdiWorld.lib"
	-@erase ".\Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O1 /I "..\ATLModel" /I "..\ATLTangramCanvas" /I "..\\" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /Fp"$(INTDIR)\ATLGdiWorld.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ATLGdiWorld.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ATLGdiWorld.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ATLGdiWorld.pdb" /machine:I386 /out:"$(OUTDIR)\ATLGdiWorld.dll" /implib:"$(OUTDIR)\ATLGdiWorld.lib" /idlout:_AtlGdiWorld.idl 
LINK32_OBJS= \
	"$(INTDIR)\ATLGdiWorld.obj" \
	"$(INTDIR)\AtlGdiWorldImpl.obj" \
	"$(INTDIR)\AtlTangramGdiVisual.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ATLGdiWorld.res" \
	"..\ATLTangramCanvas\Release\ATLTangramCanvas.lib"

"$(OUTDIR)\ATLGdiWorld.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Release
TargetPath=.\Release\ATLGdiWorld.dll
InputPath=.\Release\ATLGdiWorld.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ATLGdiWorld.dep")
!INCLUDE "ATLGdiWorld.dep"
!ELSE 
!MESSAGE Warning: cannot find "ATLGdiWorld.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ATLGdiWorld - Win32 Debug" || "$(CFG)" == "ATLGdiWorld - Win32 Release"
SOURCE=.\ATLGdiWorld.cpp

!IF  "$(CFG)" == "ATLGdiWorld - Win32 Debug"


"$(INTDIR)\ATLGdiWorld.obj"	"$(INTDIR)\ATLGdiWorld.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ATLGdiWorld.pch"


!ELSEIF  "$(CFG)" == "ATLGdiWorld - Win32 Release"


"$(INTDIR)\ATLGdiWorld.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ATLGdiWorld.pch"


!ENDIF 

SOURCE=.\ATLGdiWorld.rc

"$(INTDIR)\ATLGdiWorld.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\AtlGdiWorldImpl.cpp

!IF  "$(CFG)" == "ATLGdiWorld - Win32 Debug"


"$(INTDIR)\AtlGdiWorldImpl.obj"	"$(INTDIR)\AtlGdiWorldImpl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ATLGdiWorld.pch"


!ELSEIF  "$(CFG)" == "ATLGdiWorld - Win32 Release"


"$(INTDIR)\AtlGdiWorldImpl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ATLGdiWorld.pch"


!ENDIF 

SOURCE=.\AtlTangramGdiVisual.cpp

!IF  "$(CFG)" == "ATLGdiWorld - Win32 Debug"


"$(INTDIR)\AtlTangramGdiVisual.obj"	"$(INTDIR)\AtlTangramGdiVisual.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ATLGdiWorld.pch"


!ELSEIF  "$(CFG)" == "ATLGdiWorld - Win32 Release"


"$(INTDIR)\AtlTangramGdiVisual.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ATLGdiWorld.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ATLGdiWorld - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\ATLModel" /I "..\ATLTangramCanvas" /I "..\\" /D "_DEBUG" /D "_USRDLL" /D "WIN32" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ATLGdiWorld.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ATLGdiWorld.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ATLGdiWorld - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O1 /I "..\ATLModel" /I "..\ATLTangramCanvas" /I "..\\" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /Fp"$(INTDIR)\ATLGdiWorld.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ATLGdiWorld.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "ATLGdiWorld - Win32 Debug"

"ATLTangramCanvas - Win32 Debug" : 
   cd "\samples\attrib\atltangram\ATLTangramCanvas"
   $(MAKE) /$(MAKEFLAGS) /F .\ATLTangramCanvas.mak CFG="ATLTangramCanvas - Win32 Debug" 
   cd "..\ATLGdiWorld"

"ATLTangramCanvas - Win32 DebugCLEAN" : 
   cd "\samples\attrib\atltangram\ATLTangramCanvas"
   $(MAKE) /$(MAKEFLAGS) /F .\ATLTangramCanvas.mak CFG="ATLTangramCanvas - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\ATLGdiWorld"

!ELSEIF  "$(CFG)" == "ATLGdiWorld - Win32 Release"

"ATLTangramCanvas - Win32 Release" : 
   cd "\samples\attrib\atltangram\ATLTangramCanvas"
   $(MAKE) /$(MAKEFLAGS) /F .\ATLTangramCanvas.mak CFG="ATLTangramCanvas - Win32 Release" 
   cd "..\ATLGdiWorld"

"ATLTangramCanvas - Win32 ReleaseCLEAN" : 
   cd "\samples\attrib\atltangram\ATLTangramCanvas"
   $(MAKE) /$(MAKEFLAGS) /F .\ATLTangramCanvas.mak CFG="ATLTangramCanvas - Win32 Release" RECURSE=1 CLEAN 
   cd "..\ATLGdiWorld"

!ENDIF 


!ENDIF 

