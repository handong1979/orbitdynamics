cmake_minimum_required(VERSION 3.2)

project(OrbitDynamics)

include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/dyn)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/sofa)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(BUILD_SOFA "build sofa library" TRUE)
set(BUILD_TEST "build test" TRUE)

add_subdirectory(dyn)

if(BUILD_SOFA)
	add_subdirectory(sofa)
endif(BUILD_SOFA)

if(BUILD_TEST)
	add_subdirectory(test)
endif(BUILD_TEST)


