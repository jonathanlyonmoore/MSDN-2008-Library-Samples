# Microsoft Developer Studio Project File - Name="Bids" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 61000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Bids - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Bids.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Bids.mak" CFG="Bids - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Bids - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Bids - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Bids - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE ComPlus 0
# PROP BASE Target_Dir ""
# PROP BASE Debug_Exe ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP ComPlus 0
# PROP Target_Dir ""
# PROP Debug_Exe ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BIDS_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo
# ADD CPP /MT
# ADD CPP /W3
# ADD CPP /GX
# ADD CPP /O2
# ADD CPP /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BIDS_EXPORTS"
# ADD CPP /Yu"stdafx.h"
# ADD CPP /FD
# ADD CPP /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo
# ADD MTL /D "NDEBUG"
# ADD MTL /mktyplib203
# ADD MTL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409
# ADD RSC /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
# ADD LINK32 /nologo
# ADD LINK32 /subsystem:windows
# ADD LINK32 /dll
# ADD LINK32 /machine:I386

!ELSEIF  "$(CFG)" == "Bids - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE ComPlus 0
# PROP BASE Target_Dir ""
# PROP BASE Debug_Exe ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP ComPlus 0
# PROP Target_Dir ""
# PROP Debug_Exe ""
# ADD BASE CPP /nologo /MTd /RTC1 /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BIDS_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo
# ADD CPP /MTd
# ADD CPP /W3
# ADD CPP /Gm
# ADD CPP /GX
# ADD CPP /ZI
# ADD CPP /Od
# ADD CPP /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BIDS_EXPORTS"
# ADD CPP /Yu"stdafx.h"
# ADD CPP /FD
# ADD CPP /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo
# ADD MTL /D "_DEBUG"
# ADD MTL /mktyplib203
# ADD MTL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409
# ADD RSC /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
# ADD LINK32 /nologo
# ADD LINK32 /subsystem:windows
# ADD LINK32 /dll
# ADD LINK32 /debug
# ADD LINK32 /machine:I386

!ENDIF 

# Begin Target

# Name "Bids - Win32 Release"
# Name "Bids - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Bids.cpp
# End Source File
# Begin Source File

SOURCE=.\Items.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\user.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Bids.h
# End Source File
# Begin Source File

SOURCE=.\BidsDB.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Srf Files"

# PROP Default_Filter "srf"
# Begin Source File

SOURCE=.\addbid.srf
# End Source File
# Begin Source File

SOURCE=.\additem.srf
# End Source File
# Begin Source File

SOURCE=.\adduser.srf
# End Source File
# Begin Source File

SOURCE=.\browse.srf
# End Source File
# Begin Source File

SOURCE=.\edititem.srf
# End Source File
# Begin Source File

SOURCE=.\footer.srf
# End Source File
# Begin Source File

SOURCE=.\header.srf
# End Source File
# Begin Source File

SOURCE=.\iteminfo.srf
# End Source File
# Begin Source File

SOURCE=.\Login.srf
# End Source File
# Begin Source File

SOURCE=.\mainmenu.srf
# End Source File
# Begin Source File

SOURCE=.\myauctions.srf
# End Source File
# Begin Source File

SOURCE=.\userinfo.srf
# End Source File
# End Group
# Begin Group "Htm Files"

# PROP Default_Filter "htm"
# Begin Source File

SOURCE=.\dberror.htm
# End Source File
# Begin Source File

SOURCE=.\default.htm
# End Source File
# Begin Source File

SOURCE=.\index.htm
# End Source File
# Begin Source File

SOURCE=.\readme.htm
# End Source File
# Begin Source File

SOURCE=.\success.htm
# End Source File
# Begin Source File

SOURCE=.\usererror.htm
# End Source File
# End Group
# Begin Group "Images"

# PROP Default_Filter "jpg gif"
# Begin Source File

SOURCE=.\bidsonline.gif
# End Source File
# Begin Source File

SOURCE=.\border.png
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
