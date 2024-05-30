SOURCES=\
	.\duck.cpp	\
	.\DuckInt.cpp	\
	.\mydlg.cpp
	
PCH_FILE=StdAfx
	
RC_DEPS=\
	duck.rgs	\
	DuckInt.rgs
	
IDL_SOURCES=\
	.\duck.idl
	
RC_SOURCES=\
	.\duck.rc


TARGETNAME=duck.exe
TARGETTYPE=EXE
!include "$(SAMPDIR)\samp_inc.mak"

