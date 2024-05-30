# Microsoft Developer Studio Generated NMAKE File, Based on WebDbg.dsp
!IF "$(CFG)" == ""
CFG=WebDbg - Win32 Debug
!MESSAGE No configuration specified. Defaulting to WebDbg - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "WebDbg - Win32 Release" && "$(CFG)" != "WebDbg - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WebDbg.mak" CFG="WebDbg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WebDbg - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WebDbg - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "WebDbg - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\WebDbg.exe"


CLEAN :
	-@erase "$(INTDIR)\ChildView.obj"
	-@erase "$(INTDIR)\DebugDialog.obj"
	-@erase "$(INTDIR)\FilterDialog.obj"
	-@erase "$(INTDIR)\PipeDlg.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WebDbg.obj"
	-@erase "$(INTDIR)\WebDbg.pch"
	-@erase "$(INTDIR)\WebDbg.res"
	-@erase "$(OUTDIR)\WebDbg.exe"
	-@erase "$(OUTDIR)\WebDbg.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W4 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\WebDbg.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\WebDbg.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WebDbg.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\WebDbg.pdb" /debug /machine:I386 /out:"$(OUTDIR)\WebDbg.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ChildView.obj" \
	"$(INTDIR)\DebugDialog.obj" \
	"$(INTDIR)\FilterDialog.obj" \
	"$(INTDIR)\PipeDlg.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WebDbg.obj" \
	"$(INTDIR)\WebDbg.res"

"$(OUTDIR)\WebDbg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "WebDbg - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\WebDbg.exe"


CLEAN :
	-@erase "$(INTDIR)\ChildView.obj"
	-@erase "$(INTDIR)\DebugDialog.obj"
	-@erase "$(INTDIR)\FilterDialog.obj"
	-@erase "$(INTDIR)\PipeDlg.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WebDbg.obj"
	-@erase "$(INTDIR)\WebDbg.pch"
	-@erase "$(INTDIR)\WebDbg.res"
	-@erase "$(OUTDIR)\WebDbg.exe"
	-@erase "$(OUTDIR)\WebDbg.ilk"
	-@erase "$(OUTDIR)\WebDbg.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /RTC1 /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\WebDbg.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\WebDbg.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WebDbg.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\WebDbg.pdb" /debug /machine:I386 /out:"$(OUTDIR)\WebDbg.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ChildView.obj" \
	"$(INTDIR)\DebugDialog.obj" \
	"$(INTDIR)\FilterDialog.obj" \
	"$(INTDIR)\PipeDlg.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WebDbg.obj" \
	"$(INTDIR)\WebDbg.res"

"$(OUTDIR)\WebDbg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("WebDbg.dep")
!INCLUDE "WebDbg.dep"
!ELSE 
!MESSAGE Warning: cannot find "WebDbg.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "WebDbg - Win32 Release" || "$(CFG)" == "WebDbg - Win32 Debug"
SOURCE=.\ChildView.cpp

"$(INTDIR)\ChildView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebDbg.pch"


SOURCE=.\DebugDialog.cpp

"$(INTDIR)\DebugDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebDbg.pch"


SOURCE=.\FilterDialog.cpp

"$(INTDIR)\FilterDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebDbg.pch"

SOURCE=.\PipeDlg.cpp

"$(INTDIR)\PipeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebDbg.pch"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebDbg.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "WebDbg - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\WebDbg.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\WebDbg.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "WebDbg - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /RTC1 /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\WebDbg.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\WebDbg.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\WebDbg.cpp

"$(INTDIR)\WebDbg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebDbg.pch"


SOURCE=.\WebDbg.rc

"$(INTDIR)\WebDbg.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

