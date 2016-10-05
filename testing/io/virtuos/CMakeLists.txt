#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(VIRTUOSIO_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbVirtuosIOTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------
ADD_TEST(VirtuosDoseAccessorGeneratorTest ${VIRTUOSIO_TEST} VirtuosDoseAccessorGeneratorTest
"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.pln" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.dos.gz")

ADD_TEST(VirtuosDoseIOTest ${VIRTUOSIO_TEST} VirtuosDoseIOTest
"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.pln" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.dos.gz")

ADD_TEST(VirtuosStructureIOTest ${VIRTUOSIO_TEST} VirtuosStructureIOTest
"${TEST_DATA_ROOT}/Virtuos/NHH030/NHH030000.vdx" "${TEST_DATA_ROOT}/Virtuos/NHH030/NHH030000.ctx.gz"
"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.ctx.gz"
"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.dos.gz")

ADD_TEST(VirtuosStructureSetGeneratorTest ${VIRTUOSIO_TEST} VirtuosStructureSetGeneratorTest
"${TEST_DATA_ROOT}/Virtuos/NHH030/NHH030000.vdx" "${TEST_DATA_ROOT}/Virtuos/NHH030/NHH030000.ctx.gz"
"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx" "${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.ctx.gz"
)

#trip data
ADD_TEST(TripStructureIOTest ${VIRTUOSIO_TEST} TripStructureIOTest
"${TEST_DATA_ROOT}/Virtuos/NHH030/NHH030000.vdx" "${TEST_DATA_ROOT}/Virtuos/NHH030/NHH030000.ctx.gz" "${TEST_DATA_ROOT}/Virtuos/NHH030/NHH030000.ctx"
"${TEST_DATA_ROOT}/Virtuos/NHH030/NHH030101.dos.gz" "${TEST_DATA_ROOT}/Virtuos/NHHTrip/NHH030101.dos")

ADD_TEST(TripDoseIOTest ${VIRTUOSIO_TEST} TripDoseIOTest
"${TEST_DATA_ROOT}/Virtuos/NHH030/NHH030101.pln" "${TEST_DATA_ROOT}/Virtuos/NHH030/NHH030101.dos.gz" "${TEST_DATA_ROOT}/Virtuos/NHHTrip/NHH030101.dos")

ADD_TEST(VirtuosDVHCalculatorExampleTest ${VIRTUOSIO_TEST} VirtuosDVHCalculatorExampleTest
"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.dos.gz"
"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.vdx"
"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac101.pln"
"${TEST_DATA_ROOT}/Virtuos/prostate_ac/prostate_ac000.ctx.gz" )

RTTB_CREATE_TEST_MODULE(rttbVirtuosIO DEPENDS RTTBVirtuosIO RTTBMasks RTTBBoostMask PACKAGE_DEPENDS Boost Litmus VirtuosIO)
IF (NOT WIN32)
	#CMake 3.1 provides target_compile_features(RTTB_Interpolation cxx_auto_type cxx_nullptr cxx_override) to 			automatically add required compiler flags
	set(CMAKE_CXX_FLAGS "-std=c++11")
ENDIF()

