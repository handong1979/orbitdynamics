set(SRC
 armadillo.h
 armadillo_BLAS_LAPACK.h
 armadillo_MKL.h
 BaseException.h
 CDatetime.h
 CEngine.h
 CentralBody.h
 ColorNoise.h
 Config.h
 ConsoleUtility.h
 Constant.h
 Coordination.h
 DE405.h
 Delay.h
 Euler.h
 facility.h
 FileUtility.h
 Kepler.h
 LibrationSat.h
 Maneuver.h
 MatlabEngine.h
 Msise00.h
 nrlmsise-00.h
 OrbitDyn.h
 PerfTimer.h
 PlanetEphemeris.h
 PosVel.h
 Propagator.h
 Quaternion.h
 Satellite.h
 SimDataFile.h
 sofa.h
 sofam.h
 StopCondition.h
 StringUtility.h
 Utility.h
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

if(APPLE)

  set(ARMA_OS macos)

  set(ARMA_USE_LAPACK true)
  set(ARMA_USE_BLAS   true)

  find_library(ACCELERATE Accelerate)
  if (NOT ACCELERATE)
    message(FATAL_ERROR "Accelerate.Framework not found")
  endif()
  target_link_libraries(OrbitDyn ${ACCELERATE})
endif(APPLE)


