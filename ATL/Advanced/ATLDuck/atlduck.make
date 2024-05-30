SOURCES=\
	.\atlduck.cpp	\
	.\DuckDoer.cpp	\
	.\DuckDoerDlg.cpp
	
PCH_FILE=StdAfx
	
RC_DEPS=\
	atlduck.rgs	\
	DuckDoer.rgs
	
IDL_SOURCES=\
	.\atlduck.idl
	
RC_SOURCES=\
	.\atlduck.rc


TARGETNAME=atlduck.exe
TARGETTYPE=EXE
!include "$(SAMPDIR)\samp_inc.mak"

