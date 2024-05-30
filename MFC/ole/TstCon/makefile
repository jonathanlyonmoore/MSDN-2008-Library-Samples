######################################################################################################################
######################################################################################################################
######################################################################################################################
######################################################################################################################
######################################################################################################################
######################################################################################################################
######################################################################################################################
######################################################################################################################
# DO NOT TOUCH THIS SECTION
# -------------------------
!if "$(RUNTIME)" == "IJW"
RUNTIME_OPT =  /EHa /TP /CLR  
!endif


!IF "$(DEBUG)" == ""
DEBUG=1
!ENDIF

!IF "$(UNICODE)" == ""
UNICODE=0
!ENDIF

!IF "$(AFXDLL)" == ""
AFXDLL=1
!ENDIF

!IF "$(DEBUG)" == "1"
OUTDIR=.\Debug
!ELSE
OUTDIR=.\Release
!ENDIF

DEFAULT_COMP_OPT=/nologo /W3 /c /Wp64 /TP /EHsc
DEFAULT_LINK_OPT=/NOLOGO
CONST_MAIN_COMP_OPT=
CONST_MAIN_LINK_OPT=
CONST_MAIN_RC_OPT=


!IF "$(AFXDLL)" == "1"
CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT) /D _AFXDLL 
CONST_MAIN_RC_OPT=$(CONST_MAIN_RC_OPT) /d _AFXDLL 
!ELSE
CONST_MAIN_LINK_OPT=$(CONST_MAIN_LINK_OPT) /DELAYLOAD:"OleAcc.dll" DelayImp.lib DelayImp.lib
!ENDIF

!IF "$(UNICODE)" == "1"
CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT) /D _UNICODE /D UNICODE
!ELSE
CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT) /D _MBCS 
!ENDIF

!IF "$(DEBUG)" == "1"
CONST_MAIN_RC_OPT=$(CONST_MAIN_RC_OPT) /d _DEBUG 
CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT) /Od /D _DEBUG 
!ELSE
# CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT) /D NDEBUG
CONST_MAIN_LINK_OPT=$(CONST_MAIN_LINK_OPT) /INCREMENTAL:NO /OPT:ICF
!ENDIF

# Both DEBUG & RELEASE MODES SHOULD ADD THESE FLAGS:
CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT) /Zi
CONST_MAIN_LINK_OPT=$(CONST_MAIN_LINK_OPT) /DEBUG


!IF "$(AFXDLL)" == "1"
CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT) /MD
!IF "$(DEBUG)" == "1"
CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT)d
!ENDIF
!ELSE
CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT) /MT
!IF "$(DEBUG)" == "1"
CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT)d
!ENDIF
!ENDIF


!IF "$(PLATFORM)" == "IA64"
CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT) /D _IA64
CONST_MAIN_LINK_OPT=$(CONST_MAIN_LINK_OPT) /MACHINE:IA64
WINCONF=WIN64
!ELSEIF "$(PLATFORM)" == "AMD64"
CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT) /D _AMD64
CONST_MAIN_LINK_OPT=$(CONST_MAIN_LINK_OPT) /MACHINE:AMD64
WINCONF=WIN64
!ELSE
CONST_MAIN_LINK_OPT=$(CONST_MAIN_LINK_OPT) /MACHINE:x86
WINCONF=WIN32
!ENDIF

CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT) /D $(WINCONF)

######################################################################################################################
######################################################################################################################
######################################################################################################################
######################################################################################################################
######################################################################################################################
######################################################################################################################
######################################################################################################################
######################################################################################################################
######################################################################################################################
######################################################################################################################

# MAIN_APP_TYPE can be CONSOLE/WINDOWS
MAIN_APP_TYPE=WINDOWS
MAIN_SOURCES=*.cpp
MAIN_ADDITIONAL_COMP_OPT=
MAIN_ADDITIONAL_LINK_OPT=/OPT:REF
MAIN_IMPORT_LIB=
MAIN_RC_FILE=TstCon.rc
MAIN_RES_FILE=TstCon.res
EXE_TO_RUN=TstCon32.exe
MAIN_PROG_NAME=$(EXE_TO_RUN)



# Do not change these lines
CONST_MAIN_COMP_OPT=$(CONST_MAIN_COMP_OPT) /D _$(MAIN_APP_TYPE)
CONST_MAIN_LINK_OPT=$(CONST_MAIN_LINK_OPT) /SUBSYSTEM:$(MAIN_APP_TYPE)
!IF "$(MAIN_APP_TYPE)" == "WINDOWS" && "$(UNICODE)" == "1"
CONST_MAIN_LINK_OPT=$(CONST_MAIN_LINK_OPT) /ENTRY:"wWinMainCRTStartup"
!ENDIF


#############
# R U L E S #
#############

ALL : PREPARE_OUTDIR DLL_PROP MAIN_PROG 

PREPARE_OUTDIR:
	if not exist "$(OUTDIR)\$(NULL)" mkdir "$(OUTDIR)"

MAIN_PROG:
	midl.exe TstCon.odl /tlb "TstCon.tlb" /h "TstConIF.h" /iid "TstCon_i.c"
	cl.exe $(CONST_MAIN_COMP_OPT) $(MAIN_ADDITIONAL_COMP_OPT) $(DEFAULT_COMP_OPT) $(RUNTIME_OPT) $(MAIN_SOURCES)
	move *.obj $(OUTDIR)
	rc.exe $(CONST_MAIN_RC_OPT) /fo$(OUTDIR)\$(MAIN_RES_FILE) $(MAIN_RC_FILE)
	link.exe $(CONST_MAIN_LINK_OPT) $(MAIN_ADDITIONAL_LINK_OPT) $(DEFAULT_LINK_OPT) $(OUTDIR)\*.obj \
		 $(OUTDIR)\$(MAIN_RES_FILE) $(MAIN_IMPORT_LIB) /OUT:$(OUTDIR)\$(MAIN_PROG_NAME)
DLL_PROP:
	cd TCProps
	if not exist "$(OUTDIR)\$(NULL)" mkdir "$(OUTDIR)"
	midl.exe TCProps.idl /tlb "TCProps.tlb" /h "TCProps.h" /iid "TCProps_i.c"
	cl.exe $(CONST_MAIN_COMP_OPT) $(MAIN_ADDITIONAL_COMP_OPT) $(DEFAULT_COMP_OPT) $(RUNTIME_OPT) $(MAIN_SOURCES)
	move *.obj $(OUTDIR)
	rc.exe $(CONST_MAIN_RC_OPT) /fo$(OUTDIR)\TCProps.res TCProps.rc
	link.exe $(CONST_MAIN_LINK_OPT) $(MAIN_ADDITIONAL_LINK_OPT) $(DEFAULT_LINK_OPT) $(OUTDIR)\*.obj \
		 $(OUTDIR)\TCProps.res  /DLL $(MAIN_IMPORT_LIB) /OUT:$(OUTDIR)\TCProps.dll

	copy $(OUTDIR)\TCProps.dll ..\$(OUTDIR)
	copy TCProps.tlb ..\$(OUTDIR)
	copy TCProps.tlb ..
	copy $(OUTDIR)\TCProps.pdb ..\$(OUTDIR)
	cd ..


CLEAN :
	- @echo removing...
	- @del /f /q /s $(OUTDIR)\*.*
	- @echo done removing
