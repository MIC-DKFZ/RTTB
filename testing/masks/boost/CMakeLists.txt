#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(Boost_Mask_TESTS ${EXECUTABLE_OUTPUT_PATH}/rttbBoostMaskTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

ADD_TEST(BoostMaskTest ${Boost_Mask_TESTS} BoostMaskTest 
)

RTTB_CREATE_TEST_MODULE(rttbBoostMask DEPENDS RTTBDicomIO RTTBMasks RTTBBoostMask PACKAGE_DEPENDS BoostBinaries Litmus DCMTK)
IF (NOT WIN32)
	set(CMAKE_CXX_FLAGS "-std=c++11")
ENDIF()
