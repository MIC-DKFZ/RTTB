#-----------------------------------------------------------------------------
# Setup the system information test.  Write out some basic failsafe
# information in case the test doesn't run.
#-----------------------------------------------------------------------------


SET(OTHERIO_TEST ${EXECUTABLE_OUTPUT_PATH}/rttbOtherIOTests)

SET(TEST_DATA_ROOT ${RTTBTesting_SOURCE_DIR}/data)

SET(TEMP ${RTTBTesting_BINARY_DIR}/temporary)


#-----------------------------------------------------------------------------

ADD_TEST(DoseStatisticsIOTest ${OTHERIO_TEST} DoseStatisticsIOTest) 

ADD_TEST(DVHXMLIOTest ${OTHERIO_TEST} DVHXMLIOTest "${TEST_DATA_ROOT}/TestDVH/dvh_1.txt") 

ADD_TEST(OtherIOTest ${OTHERIO_TEST} OtherIOTest) 

RTTB_CREATE_TEST_MODULE(rttbOtherIO DEPENDS RTTBOtherIO PACKAGE_DEPENDS Boost Litmus)

