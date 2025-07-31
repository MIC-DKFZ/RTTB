#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------

SET(OTHERIO_TEST ${EXECUTABLE_OUTPUT_PATH}/${RTToolbox_PREFIX}OtherIOTests)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

ADD_TEST(DoseStatisticsIOTest ${OTHERIO_TEST} DoseStatisticsIOTest) 

ADD_TEST(DVHXMLIOTest ${OTHERIO_TEST} DVHXMLIOTest) 

RTTB_CREATE_TEST_MODULE(OtherIO DEPENDS RTTBOtherIO RTTBTestHelper PACKAGE_DEPENDS Litmus RTTBData)
