
set (LowThrusts
    LTOrbitCal
	Ctrla
	Ctrli
	Example2
	GTO2GEO
	LEO2GEO
	LEOKeep
	lt
	LTOrbitControl
)

foreach (lt ${LowThrusts})
  add_executable(${lt} ${lt}.cpp)
  target_link_libraries(${lt} OrbitDyn)
  set_target_properties(${lt} PROPERTIES FOLDER LowThrust)
endforeach ()

add_executable(LTAttiCal LTAttiCal.cpp  AttDyn.cpp AttDyn.h)
target_link_libraries(LTAttiCal OrbitDyn)
set_target_properties(LTAttiCal PROPERTIES FOLDER LowThrust)

add_executable(LTAttiControl LTAttiControl.cpp  AttDyn.cpp AttDyn.h)
target_link_libraries(LTAttiControl OrbitDyn)
set_target_properties(LTAttiControl PROPERTIES FOLDER LowThrust)