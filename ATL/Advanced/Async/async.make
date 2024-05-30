SOURCES=\
	.\Async.cpp	\
	.\ATLAsync.cpp
	
PCH_FILE=StdAfx

IDL_SOURCES=\
	.\async.idl

RC_SOURCES=\
	.\Async.rc

DEF_FILE=Async.def
	
RC_DEPS=\
	ATLAsync.rgs
	
TARGETNAME=async.dll
TARGETTYPE=dll

!include "$(SAMPDIR)\samp_inc.mak"

