cmake_minimum_required(VERSION 3.2)

project(OrbitDynamics)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

if(APPLE)

  set(ARMA_OS macos)

  set(ARMA_USE_LAPACK true)
  set(ARMA_USE_BLAS   true)

  set(ARMA_LIBS ${ARMA_LIBS} "-framework Accelerate")  # or "-framework accelerate" ?
  message(STATUS "MacOS X detected. Added '-framework Accelerate' to compiler flags")
endif()


set(SRC
 ./include/BaseException.h
 ./include/Kepler.h
 ./include/StopCondition.h
 ./include/CDateTime.h
 ./include/LibrationSat.h
 ./include/StringUtility.h
 ./include/CEngine.h
 ./include/Maneuver.h
 ./include/Utility.h
 ./include/CentralBody.h
 ./include/MatlabEngine.h
 ./include/armadillo.h
 ./include/ColorNoise.h
 ./include/Msise00.h
 ./include/armadillo_BLAS_LAPACK.h
 ./include/Config.h
 ./include/OrbitDyn.h
 ./include/armadillo_MKL.h
 ./include/ConsoleUtility.h
 ./include/Constant.h
 ./include/PlanetEphemeris.h
 ./include/facility.h
 ./include/Coordination.h
 ./include/PosVel.h
 ./include/DE405.h
 ./include/Propagator.h
 ./include/nrlmsise-00.h
 ./include/Delay.h
 ./include/Quaternion.h
 ./include/sofa.h
 ./include/Euler.h
 ./include/Satellite.h
 ./include/sofam.h
 ./include/FileUtility.h
 ./include/SimDataFile.h
 ./src/BaseException.cpp
 ./src/CDateTime.cpp
 ./src/CEngine.cpp
 ./src/CentralBody.cpp
 ./src/Euler.cpp
 ./src/Facility.cpp
 ./src/Kepler.cpp
 ./src/Msise00.cpp
 ./src/nrlmsise-00.c
 ./src/nrlmsise-00_data.c
 ./src/PlanetEphemeris.cpp
 ./src/Propagator.cpp
 ./src/Quaternion.cpp
 ./src/Satellite.cpp
 ./src/StopCondition.cpp
 ./src/StringUtility.cpp
 ./src/Utility.cpp)

add_library(OrbitDyn SHARED ${SRC})

add_executable(test_circle_moon ./test/test_circle_moon.cpp)
add_executable(test_DE405_DE421 ./test/test_DE405_DE421.cpp)
add_executable(test_earth_lunar_transfer ./test/test_earth_lunar_transfer.cpp)
add_executable(checkmemoryleak ./test/checkmemoryleak.cpp)
add_executable(test_mean_inst ./test/test_mean_inst.cpp)
add_executable(test_orbit_propagator ./test/test_orbit_propagator.cpp)
add_executable(test_rapidsattlite ./test/test_rapidsattlite.cpp)
add_executable(test_RIC ./test/test_RIC.cpp)
add_executable(test_sg ./test/test_sg.cpp)
add_executable(test_sofa_Gst ./test/test_sofa_Gst.cpp)
add_executable(testBackward ./test/testBackward.cpp)
