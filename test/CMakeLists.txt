
set (tests
    test_circle_moon
    test_DE405_DE421
    test_earth_lunar_transfer
    test_facility
    checkmemoryleak
	test_mean_inst
	test_orbit_propagator
	test_orbit_maneuver
	test_rapidsattlite
	test_RIC
	test_sg
	test_sofa_Gst
	testBackward
	test_access
	testjson
)

foreach (test ${tests})
  add_executable (${test} ${test}.cpp)
  target_link_libraries (${test} OrbitDyn)
  set_target_properties (${test} PROPERTIES FOLDER Tests)
endforeach ()

