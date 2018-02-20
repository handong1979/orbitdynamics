set(SRC 
 BaseException.cpp
 CDateTime.cpp
 CEngine.cpp
 CentralBody.cpp
 Euler.cpp
 Facility.cpp
 Kepler.cpp
 Msise00.cpp
 nrlmsise-00.c
 nrlmsise-00_data.c
 PlanetEphemeris.cpp
 Propagator.cpp
 Quaternion.cpp
 Satellite.cpp
 StopCondition.cpp
 StringUtility.cpp
 Utility.cpp)

add_library(OrbitDyn SHARED ${SRC})