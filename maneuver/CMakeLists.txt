
set (maneuvers
    leotrans
	leocap
	leodeorbit
	sap1
	sap2
	groundtraj
	AdjustTime
	attdet
	attctrl
	consformation
	geocolocationE
	geocolocationI
	geodeorbit
	geoEWsk
	geoNSsk
	geotrans
)

foreach (maneuver ${maneuvers})
  add_executable(${maneuver} ${maneuver}.cpp loadorbit.cpp)
  target_link_libraries(${maneuver} OrbitDyn)
  set_target_properties(${maneuver} PROPERTIES FOLDER AOC)
endforeach ()

