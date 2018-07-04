cmake_minimum_required(VERSION 3.2)

project(OrbitDynamics)

include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/orbitdyn)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(BUILD_SOFA "build sofa library" TRUE)
set(BUILD_TEST "build test" TRUE)

if(APPLE)

  set(ARMA_OS macos)

  set(ARMA_USE_LAPACK true)
  set(ARMA_USE_BLAS   true)

  set(ARMA_LIBS ${ARMA_LIBS} "-framework Accelerate")  # or "-framework accelerate" ?
  message(STATUS "MacOS X detected. Added '-framework Accelerate' to compiler flags")
endif(APPLE)

add_subdirectory(orbitdyn)

if(BUILD_SOFA)
	add_subdirectory(sofa)
endif(BUILD_SOFA)

if(BUILD_TEST)
	add_subdirectory(test)
endif(BUILD_TEST)


