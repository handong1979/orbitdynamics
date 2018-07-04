set(SRC
 armadillo.h
 BaseException.h
 BaseException.cpp
 CDateTime.h
 CDateTime.cpp
 CEngine.h
 CEngine.cpp
 CentralBody.h
 CentralBody.cpp
 ColorNoise.h
 Config.h
 ConsoleUtility.h
 Constant.h
 Coordination.h
 Delay.h
 Euler.h
 Euler.cpp
 Facility.h
 Facility.cpp
 FileUtility.h
 Kepler.h
 Kepler.cpp
 LibrationSat.h
 Maneuver.h
 MatlabEngine.h
 Msise00.h
 Msise00.cpp
 nrlmsise-00.c
 nrlmsise-00_data.c
 OrbitDyn.h
 PerfTimer.h
 PlanetEphemeris.h
 PlanetEphemeris.cpp
 Propagator.h
 Propagator.cpp
 PosVel.h
 Quaternion.h
 Quaternion.cpp
 Satellite.h
 Satellite.cpp
 SimDataFile.h
 StopCondition.h
 StopCondition.cpp
 StringUtility.h
 StringUtility.cpp
 Utility.h
 Utility.cpp)

if(APPLE)

  set(ARMA_OS macos)

  set(ARMA_USE_LAPACK true)
  set(ARMA_USE_BLAS   true)

  find_library(ACCELERATE Accelerate)
  if (NOT ACCELERATE)
    message(FATAL_ERROR "Accelerate.Framework not found")
  endif()

endif(APPLE)

add_library(OrbitDyn SHARED ${SRC})
target_link_libraries(OrbitDyn ${ACCELERATE})
