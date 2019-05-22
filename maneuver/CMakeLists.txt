add_executable(leotrans leotrans.cpp)
target_link_libraries(leotrans OrbitDyn)

add_executable(leocap leocap.cpp loadorbit.cpp)
target_link_libraries(leocap OrbitDyn)

add_executable(leodeorbit leodeorbit.cpp loadorbit.cpp)
target_link_libraries(leodeorbit OrbitDyn)

add_executable(sap1 sap1.cpp loadorbit.cpp)
target_link_libraries(sap1 OrbitDyn)

add_executable(sap2 sap2.cpp loadorbit.cpp)
target_link_libraries(sap2 OrbitDyn)

add_executable(groundtraj groundtraj.cpp)
target_link_libraries(groundtraj OrbitDyn)

add_executable(AdjustTime AdjustTime.cpp loadorbit.cpp)
target_link_libraries(AdjustTime OrbitDyn)

add_executable(attdet attdet.cpp loadorbit.cpp)
target_link_libraries(attdet OrbitDyn)

add_executable(attctrl attctrl.cpp loadorbit.cpp)
target_link_libraries(attctrl OrbitDyn)

add_executable(consformation consformation.cpp loadorbit.cpp)
target_link_libraries(consformation OrbitDyn)

add_executable(geocolocationE geocolocationE.cpp loadorbit.cpp)
target_link_libraries(geocolocationE OrbitDyn)

add_executable(geocolocationI geocolocationI.cpp loadorbit.cpp)
target_link_libraries(geocolocationI OrbitDyn)

add_executable(geodeorbit geodeorbit.cpp loadorbit.cpp)
target_link_libraries(geodeorbit OrbitDyn)

add_executable(geoEWsk geoEWsk.cpp loadorbit.cpp)
target_link_libraries(geoEWsk OrbitDyn)

add_executable(geoNSsk geoNSsk.cpp loadorbit.cpp)
target_link_libraries(geoNSsk OrbitDyn)

add_executable(geotrans geotrans.cpp loadorbit.cpp)
target_link_libraries(geotrans OrbitDyn)