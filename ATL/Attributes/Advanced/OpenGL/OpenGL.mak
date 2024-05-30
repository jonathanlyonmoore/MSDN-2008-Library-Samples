# Microsoft Developer Studio Generated NMAKE File, Based on OpenGL.dsp
!IF "$(CFG)" == ""
CFG=OpenGL - Win32 Release
!MESSAGE No configuration specified. Defaulting to OpenGL - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "OpenGL - Win32 Release" && "$(CFG)" != "OpenGL - Win32 Debug" && "$(CFG)" != "OpenGL - Win32 Unicode Release" && "$(CFG)" != "OpenGL - Win32 Unicode Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OpenGL.mak" CFG="OpenGL - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OpenGL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OpenGL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OpenGL - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OpenGL - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "OpenGL - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\OpenGL.dll" "$(OUTDIR)\OpenGL.bsc" ".\Release\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\OpenGL.obj"
	-@erase "$(INTDIR)\OpenGL.pch"
	-@erase "$(INTDIR)\OpenGL.res"
	-@erase "$(INTDIR)\OpenGL.sbr"
	-@erase "$(INTDIR)\OpenGLObj.obj"
	-@erase "$(INTDIR)\OpenGLObj.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\OpenGL.bsc"
	-@erase "$(OUTDIR)\OpenGL.dll"
	-@erase "$(OUTDIR)\OpenGL.exp"
	-@erase "$(OUTDIR)\OpenGL.lib"
	-@erase ".\Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G5 /Gr /MT /W4 /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D _WIN32_WINNT=0x400 /D "_ATL_ATTRIBUTES" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\OpenGL.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\OpenGL.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\OpenGL.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\OpenGL.sbr" \
	"$(INTDIR)\OpenGLObj.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\OpenGL.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\OpenGL.pdb" /machine:I386 /out:"$(OUTDIR)\OpenGL.dll" /implib:"$(OUTDIR)\OpenGL.lib" /idlout:_opengl.idl 
LINK32_OBJS= \
	"$(INTDIR)\OpenGL.obj" \
	"$(INTDIR)\OpenGLObj.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\OpenGL.res"

"$(OUTDIR)\OpenGL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Release
TargetPath=.\Release\OpenGL.dll
InputPath=.\Release\OpenGL.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "OpenGL - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\OpenGL.dll" "$(OUTDIR)\OpenGL.bsc" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\OpenGL.obj"
	-@erase "$(INTDIR)\OpenGL.pch"
	-@erase "$(INTDIR)\OpenGL.res"
	-@erase "$(INTDIR)\OpenGL.sbr"
	-@erase "$(INTDIR)\OpenGLObj.obj"
	-@erase "$(INTDIR)\OpenGLObj.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\OpenGL.bsc"
	-@erase "$(OUTDIR)\OpenGL.dll"
	-@erase "$(OUTDIR)\OpenGL.exp"
	-@erase "$(OUTDIR)\OpenGL.ilk"
	-@erase "$(OUTDIR)\OpenGL.lib"
	-@erase "$(OUTDIR)\OpenGL.pdb"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D _WIN32_WINNT=0x400 /D "_ATL_ATTRIBUTES" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\OpenGL.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\OpenGL.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\OpenGL.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\OpenGL.sbr" \
	"$(INTDIR)\OpenGLObj.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\OpenGL.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\OpenGL.pdb" /debug /machine:I386 /out:"$(OUTDIR)\OpenGL.dll" /implib:"$(OUTDIR)\OpenGL.lib" /idlout:_opengl.idl 
LINK32_OBJS= \
	"$(INTDIR)\OpenGL.obj" \
	"$(INTDIR)\OpenGLObj.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\OpenGL.res"

"$(OUTDIR)\OpenGL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\OpenGL.dll
InputPath=.\Debug\OpenGL.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "OpenGL - Win32 Unicode Release"

OUTDIR=.\ReleaseU
INTDIR=.\ReleaseU
# Begin Custom Macros
OutDir=.\ReleaseU
# End Custom Macros

ALL : "$(OUTDIR)\OpenGL.dll" ".\ReleaseU\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\OpenGL.obj"
	-@erase "$(INTDIR)\OpenGL.pch"
	-@erase "$(INTDIR)\OpenGL.res"
	-@erase "$(INTDIR)\OpenGLObj.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\OpenGL.dll"
	-@erase "$(OUTDIR)\OpenGL.exp"
	-@erase "$(OUTDIR)\OpenGL.lib"
	-@erase ".\ReleaseU\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W4 /GX /O2 /D "NDEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D _WIN32_WINNT=0x400 /D "_ATL_ATTRIBUTES" /Fp"$(INTDIR)\OpenGL.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\OpenGL.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\OpenGL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\OpenGL.pdb" /machine:I386 /out:"$(OUTDIR)\OpenGL.dll" /implib:"$(OUTDIR)\OpenGL.lib" /idlout:_opengl.idl 
LINK32_OBJS= \
	"$(INTDIR)\OpenGL.obj" \
	"$(INTDIR)\OpenGLObj.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\OpenGL.res"

"$(OUTDIR)\OpenGL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\ReleaseU
TargetPath=.\ReleaseU\OpenGL.dll
InputPath=.\ReleaseU\OpenGL.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "OpenGL - Win32 Unicode Debug"

OUTDIR=.\DebugU
INTDIR=.\DebugU
# Begin Custom Macros
OutDir=.\DebugU
# End Custom Macros

ALL : "$(OUTDIR)\OpenGL.dll" ".\DebugU\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\OpenGL.obj"
	-@erase "$(INTDIR)\OpenGL.pch"
	-@erase "$(INTDIR)\OpenGL.res"
	-@erase "$(INTDIR)\OpenGLObj.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\OpenGL.dll"
	-@erase "$(OUTDIR)\OpenGL.exp"
	-@erase "$(OUTDIR)\OpenGL.ilk"
	-@erase "$(OUTDIR)\OpenGL.lib"
	-@erase "$(OUTDIR)\OpenGL.pdb"
	-@erase ".\DebugU\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D _WIN32_WINNT=0x400 /D "_ATL_ATTRIBUTES" /Fp"$(INTDIR)\OpenGL.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\OpenGL.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\OpenGL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\OpenGL.pdb" /debug /machine:I386 /out:"$(OUTDIR)\OpenGL.dll" /implib:"$(OUTDIR)\OpenGL.lib" /idlout:_opengl.idl 
LINK32_OBJS= \
	"$(INTDIR)\OpenGL.obj" \
	"$(INTDIR)\OpenGLObj.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\OpenGL.res"

"$(OUTDIR)\OpenGL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\DebugU
TargetPath=.\DebugU\OpenGL.dll
InputPath=.\DebugU\OpenGL.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("OpenGL.dep")
!INCLUDE "OpenGL.dep"
!ELSE 
!MESSAGE Warning: cannot find "OpenGL.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "OpenGL - Win32 Release" || "$(CFG)" == "OpenGL - Win32 Debug" || "$(CFG)" == "OpenGL - Win32 Unicode Release" || "$(CFG)" == "OpenGL - Win32 Unicode Debug"
SOURCE=.\OpenGL.cpp

!IF  "$(CFG)" == "OpenGL - Win32 Release"


"$(INTDIR)\OpenGL.obj"	"$(INTDIR)\OpenGL.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\OpenGL.pch"


!ELSEIF  "$(CFG)" == "OpenGL - Win32 Debug"


"$(INTDIR)\OpenGL.obj"	"$(INTDIR)\OpenGL.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\OpenGL.pch"


!ELSEIF  "$(CFG)" == "OpenGL - Win32 Unicode Release"


"$(INTDIR)\OpenGL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\OpenGL.pch"


!ELSEIF  "$(CFG)" == "OpenGL - Win32 Unicode Debug"


"$(INTDIR)\OpenGL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\OpenGL.pch"


!ENDIF 

SOURCE=.\OpenGL.rc

"$(INTDIR)\OpenGL.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\OpenGLObj.cpp

!IF  "$(CFG)" == "OpenGL - Win32 Release"


"$(INTDIR)\OpenGLObj.obj"	"$(INTDIR)\OpenGLObj.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\OpenGL.pch"


!ELSEIF  "$(CFG)" == "OpenGL - Win32 Debug"


"$(INTDIR)\OpenGLObj.obj"	"$(INTDIR)\OpenGLObj.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\OpenGL.pch"


!ELSEIF  "$(CFG)" == "OpenGL - Win32 Unicode Release"


"$(INTDIR)\OpenGLObj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\OpenGL.pch"


!ELSEIF  "$(CFG)" == "OpenGL - Win32 Unicode Debug"


"$(INTDIR)\OpenGLObj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\OpenGL.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "OpenGL - Win32 Release"

CPP_SWITCHES=/nologo /G5 /Gr /MT /W4 /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D _WIN32_WINNT=0x400 /D "_ATL_ATTRIBUTES" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\OpenGL.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\OpenGL.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "OpenGL - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D _WIN32_WINNT=0x400 /D "_ATL_ATTRIBUTES" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\OpenGL.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\OpenGL.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "OpenGL - Win32 Unicode Release"

CPP_SWITCHES=/nologo /MT /W4 /GX /O2 /D "NDEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D _WIN32_WINNT=0x400 /D "_ATL_ATTRIBUTES" /Fp"$(INTDIR)\OpenGL.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\OpenGL.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "OpenGL - Win32 Unicode Debug"

CPP_SWITCHES=/nologo /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D _WIN32_WINNT=0x400 /D "_ATL_ATTRIBUTES" /Fp"$(INTDIR)\OpenGL.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\OpenGL.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

