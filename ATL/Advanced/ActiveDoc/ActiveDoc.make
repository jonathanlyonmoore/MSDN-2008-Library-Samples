SOURCES=\
	.\ActiveCtl.cpp	\
	.\ActiveDoc.cpp
	
PCH_FILE=StdAfx

IDL_SOURCES=.\ActiveDoc.idl	

RC_SOURCES=.\ActiveDoc.rc

RC_DEPS=ActiveDoc.rgs ActiveDoc.tlb toolbar.bmp

DEF_FILE=ActiveDoc.def

LINK_LIBS=comctl32.lib 

TARGETNAME=ActiveDoc.dll
TARGETTYPE=dll

!include "$(SAMPDIR)\samp_inc.mak"