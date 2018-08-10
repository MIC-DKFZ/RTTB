#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(Boost_Mask_TESTS ${EXECUTABLE_OUTPUT_PATH}/${RTToolbox_PREFIX}BoostMaskTests)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

ADD_TEST(BoostMaskTest ${Boost_Mask_TESTS} BoostMaskTest)

RTTB_CREATE_TEST_MODULE(Mask DEPENDS RTTBDicomIO RTTBMask PACKAGE_DEPENDS BoostBinaries Litmus DCMTK)

IF (CMAKE_COMPILER_IS_GNUCC)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals")
ENDIF()