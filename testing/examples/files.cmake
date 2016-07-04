SET(DOSESTATISTICSVIRTUOSTEST "")
IF(RTTB_VIRTUOS_SUPPORT AND BUILD_IO_Virtuos)
	SET(DOSESTATISTICSVIRTUOSTEST "RTDoseStatisticsVirtuosTest.cpp")
ENDIF()
	
SET(CPP_FILES
	RTBioModelExampleTest.cpp
	RTBioModelScatterPlotExampleTest.cpp
	RTDoseIndexTest.cpp
	RTDoseStatisticsDicomTest.cpp
	${DOSESTATISTICSVIRTUOSTEST}
	RTDVHTest.cpp
	DVHCalculatorExampleTest.cpp
	DVHCalculatorComparisonTest.cpp
	../models/rttbScatterTester.cpp
	rttbTestExamples.cpp
	VoxelizationValidationTest.cpp
  )

SET(H_FILES
	../models/rttbScatterTester.h

)
