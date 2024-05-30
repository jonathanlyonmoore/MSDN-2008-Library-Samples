SOURCES=\
	.\Connect.cpp	\
	.\Random.cpp
	
PCH_FILE=preconn

RC_DEPS=\
	Random.rgs
	
DEF_FILE=Connect.def

IDL_SOURCES=\
	.\Connect.idl
	
RC_SOURCES=\
	.\Connect.rc
	


TARGETNAME=connect.dll
TARGETTYPE=dll

!include "$(SAMPDIR)\samp_inc.mak"

