add_executable(rendezvous rendezvous.cpp)
target_link_libraries(rendezvous OrbitDyn)

add_executable(sma sma.cpp)
target_link_libraries(sma OrbitDyn)

add_executable(incl incl.cpp)
target_link_libraries(incl OrbitDyn)

add_executable(groundtraj groundtraj.cpp)
target_link_libraries(groundtraj OrbitDyn)