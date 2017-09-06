# Microsoft Developer Studio Project File - Name="VC6OrbitDyn" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=VC6OrbitDyn - Win32 DebugMTd
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VC6OrbitDyn.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VC6OrbitDyn.mak" CFG="VC6OrbitDyn - Win32 DebugMTd"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VC6OrbitDyn - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "VC6OrbitDyn - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "VC6OrbitDyn - Win32 ReleaseMTD" (based on "Win32 (x86) Static Library")
!MESSAGE "VC6OrbitDyn - Win32 DebugMTDd" (based on "Win32 (x86) Static Library")
!MESSAGE "VC6OrbitDyn - Win32 ReleaseMT" (based on "Win32 (x86) Static Library")
!MESSAGE "VC6OrbitDyn - Win32 DebugMTd" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VC6OrbitDyn - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /compile_only /nologo /warn:nofileopt
# ADD F90 /compile_only /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "ORBITDYN_STATIC" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\msvc60\OrbitDynSR.lib"

!ELSEIF  "$(CFG)" == "VC6OrbitDyn - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "ORBITDYN_STATIC" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\msvc60\OrbitDynSD.lib"

!ELSEIF  "$(CFG)" == "VC6OrbitDyn - Win32 ReleaseMTD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VC6OrbitDyn___Win32_ReleaseMTD"
# PROP BASE Intermediate_Dir "VC6OrbitDyn___Win32_ReleaseMTD"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "VC6OrbitDyn___Win32_ReleaseMTD"
# PROP Intermediate_Dir "VC6OrbitDyn___Win32_ReleaseMTD"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /compile_only /nologo /warn:nofileopt
# ADD F90 /compile_only /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_DLL" /D "_MT" /D "ORBITDYN_STATIC" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\msvc60\OrbitDynMDllR.lib"

!ELSEIF  "$(CFG)" == "VC6OrbitDyn - Win32 DebugMTDd"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VC6OrbitDyn___Win32_DebugMTDd"
# PROP BASE Intermediate_Dir "VC6OrbitDyn___Win32_DebugMTDd"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "VC6OrbitDyn___Win32_DebugMTDd"
# PROP Intermediate_Dir "VC6OrbitDyn___Win32_DebugMTDd"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_DLL" /D "_MT" /D "ORBITDYN_STATIC" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\msvc60\OrbitDynMDllD.lib"

!ELSEIF  "$(CFG)" == "VC6OrbitDyn - Win32 ReleaseMT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VC6OrbitDyn___Win32_ReleaseMT"
# PROP BASE Intermediate_Dir "VC6OrbitDyn___Win32_ReleaseMT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "VC6OrbitDyn___Win32_ReleaseMT"
# PROP Intermediate_Dir "VC6OrbitDyn___Win32_ReleaseMT"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /compile_only /nologo /warn:nofileopt
# ADD F90 /compile_only /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "ORBITDYN_STATIC" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\msvc60\OrbitDynMR.lib"

!ELSEIF  "$(CFG)" == "VC6OrbitDyn - Win32 DebugMTd"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VC6OrbitDyn___Win32_DebugMTd"
# PROP BASE Intermediate_Dir "VC6OrbitDyn___Win32_DebugMTd"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "VC6OrbitDyn___Win32_DebugMTd"
# PROP Intermediate_Dir "VC6OrbitDyn___Win32_DebugMTd"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "ORBITDYN_STATIC" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\msvc60\OrbitDynMD.lib"

!ENDIF 

# Begin Target

# Name "VC6OrbitDyn - Win32 Release"
# Name "VC6OrbitDyn - Win32 Debug"
# Name "VC6OrbitDyn - Win32 ReleaseMTD"
# Name "VC6OrbitDyn - Win32 DebugMTDd"
# Name "VC6OrbitDyn - Win32 ReleaseMT"
# Name "VC6OrbitDyn - Win32 DebugMTd"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=..\..\src\BaseException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\CDateTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\CEngine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\CentralBody.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\PlanetEphemeris.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Euler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Facility.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Kepler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\MatrixTCL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Msise00.cpp
# End Source File
# Begin Source File

SOURCE="..\..\src\nrlmsise-00.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\nrlmsise-00_data.c"
# End Source File
# Begin Source File

SOURCE=..\..\src\Quaternion.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Satellite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\StopCondition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Utility.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Vector.cpp
# End Source File
# End Group
# Begin Group "Include Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\..\include\BaseException.h
# End Source File
# Begin Source File

SOURCE=..\..\include\CDatetime.h
# End Source File
# Begin Source File

SOURCE=..\..\include\CEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\include\CentralBody.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Config.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Constant.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Coordination.h
# End Source File
# Begin Source File

SOURCE=..\..\include\PlanetEphemeris.h
# End Source File
# Begin Source File
SOURCE=..\..\include\Euler.h
# End Source File
# Begin Source File

SOURCE=..\..\include\facility.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HDFCL.H
# End Source File
# Begin Source File

SOURCE=..\..\include\Kepler.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Maneuver.h
# End Source File
# Begin Source File

SOURCE=..\..\include\matrixtcl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Msise00.h
# End Source File
# Begin Source File

SOURCE="..\..\include\nrlmsise-00.h"
# End Source File
# Begin Source File

SOURCE=..\..\include\OrbitDyn.h
# End Source File
# Begin Source File

SOURCE=..\..\include\PerfTimer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Quaternion.h
# End Source File
# Begin Source File

SOURCE=..\..\include\satellite.h
# End Source File
# Begin Source File

SOURCE=..\..\include\StopCondition.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Utility.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Vector.h
# End Source File
# End Group
# End Target
# End Project
