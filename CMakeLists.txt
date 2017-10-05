cmake_minimum_required(VERSION 3.2)

project(OrbitDynamics)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(SRC ./src/BaseException.cpp ./src/CDateTime.cpp ./src/CEngine.cpp ./src/CentralBody.cpp ./src/Euler.cpp ./src/Facility.cpp
        ./src/Kepler.cpp ./src/Msise00.cpp ./src/nrlmsise-00.c ./src/nrlmsise-00_data.c
        ./src/PlanetEphemeris.cpp ./src/Propagator.cpp ./src/Quaternion.cpp ./src/Satellite.cpp  ./src/StopCondition.cpp
        ./src/StringUtility.cpp  ./src/Utility.cpp)

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
