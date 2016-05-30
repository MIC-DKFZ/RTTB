

SET(CPP_FILES
	DoseToolBasicUsageTest.cpp
	DoseToolInvalidParametersTest.cpp
	DoseToolDicomDoseTest.cpp
	DoseToolITKDoseTest.cpp
	DoseToolRegexTest.cpp
	DoseToolDVHTest.cpp
	DoseToolTests.cpp
  )
  
IF (RTTB_VIRTUOS_SUPPORT AND BUILD_IO_Virtuos)
	SET(CPP_FILES ${CPP_FILES} DoseToolVirtuosDoseTest.cpp)
ENDIF()

SET(H_FILES
)
