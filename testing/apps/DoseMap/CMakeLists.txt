#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(DOSEMAP_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbDoseMapTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

IF(MSVC)
     ADD_DEFINITIONS(/bigobj)
ENDIF()

IF (WIN32)
	SET(DOSEMAPEXE "DoseMap.exe")
ELSE (WIN32)
	SET(DOSEMAPEXE "./DoseMap")
ENDIF (WIN32)

ADD_TEST(DoseMapInvalidParametersTest ${DOSEMAP_TEST} DoseMapInvalidParametersTest ${DOSEMAPEXE})
ADD_TEST(DoseMapSimpleTest ${DOSEMAP_TEST} DoseMapSimpleTest ${DOSEMAPEXE} "${TEST_DATA_ROOT}/DoseMap/input.mhd" "${TEST_DATA_ROOT}/DoseMap/outputfile.mhd" "itk" "${TEST_DATA_ROOT}/DoseMap/Identity.mapr"  )


RTTB_CREATE_APPLICATION_TESTS(DoseMap PACKAGE_DEPENDS Litmus BoostBinaries)
