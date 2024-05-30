SOURCES=\
	.\atlbutn.cpp	\
	.\AtlButton.cpp
	
PCH_FILE=StdAfx

DEF_FILE=atlbutn.def

IDL_SOURCES=\
	.\atlbutn.idl
	
RC_SOURCES=\
	atlbutn.rc
	
RC_DEPS=\
	AtlButton.rgs

TARGETNAME=atlbutn.dll
TARGETTYPE=dll

!include "$(SAMPDIR)\samp_inc.mak"

