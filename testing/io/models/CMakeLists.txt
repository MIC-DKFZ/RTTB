#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(MODELSIO_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbModelsIOTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

ADD_TEST(ModelsIOTest ${MODELSIO_TEST} ModelsIOTest "${TEST_DATA_ROOT}/XML/BioModel/referenceBioModeltcpleqIOTest.xml" "${TEST_DATA_ROOT}/XML/BioModel/referenceBioModelntcplkIOTest.xml" ) 



RTTB_CREATE_TEST_MODULE(rttbModelsIO DEPENDS RTTBModelsIO RTTBModels PACKAGE_DEPENDS Boost Litmus)
IF (NOT WIN32)
	#CMake 3.1 provides target_compile_features(RTTB_Interpolation cxx_auto_type cxx_nullptr cxx_override) to 			automatically add required compiler flags
	set(CMAKE_CXX_FLAGS "-std=c++11")
ENDIF()
