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

if(APPLE)

  set(ARMA_OS macos)

  set(ARMA_USE_LAPACK true)
  set(ARMA_USE_BLAS   true)

#set(ARMA_LIBS ${ARMA_LIBS} "-framework Accelerate")  # or "-framework accelerate" ?
#message(STATUS "MacOS X detected. Added '-framework Accelerate' to compiler flags")

    find_library(ACC Accelerate)
    if (NOT ACC)
        message(FATAL_ERROR "Accelerate.Framework not found")
    endif()

endif(APPLE)

add_library(OrbitDyn SHARED ${SRC})
target_link_libraries(OrbitDyn ${ACC})
