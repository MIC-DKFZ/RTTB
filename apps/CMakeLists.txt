MESSAGE(STATUS "processing RTToolbox apps")


#extract and build ArgumentParsingLib
include(ExternalProject)
message(STATUS "ArgumentParsingLib will be automatically downloaded and built.")
set(ArgumentParsingLib_SOURCE_DIR "${CMAKE_BINARY_DIR}/external/ArgumentParsingLib-src")
set(ArgumentParsingLib_BUILD_DIR "${CMAKE_BINARY_DIR}/external/ArgumentParsingLib-build")
set(ArgumentParsingLib_CMAKE_DIR "${CMAKE_BINARY_DIR}/external/ArgumentParsingLib-cmake")


IF(BUILD_SHARED_LIBS OR RTTB_USE_MITK_BOOST)
  SET(ARG_BUILD_SHARED_LIBS ON)
ELSE(BUILD_SHARED_LIBS OR RTTB_USE_MITK_BOOST)
  SET(ARG_BUILD_SHARED_LIBS OFF)
ENDIF(BUILD_SHARED_LIBS OR RTTB_USE_MITK_BOOST)

ExternalProject_Add(
	ArgumentParsingLib
	URL ${RTToolbox_SOURCE_DIR}/utilities/ArgumentParsingLib/ArgumentParsingLib.tar.gz
	URL_HASH SHA1=6E55D67860D3F571D5EAB393F4AF70AD63DAE377
	SOURCE_DIR ${ArgumentParsingLib_SOURCE_DIR}
	BINARY_DIR ${ArgumentParsingLib_BUILD_DIR}
	PREFIX ${ArgumentParsingLib_CMAKE_DIR}
	INSTALL_COMMAND ""
	UPDATE_COMMAND ""
	CMAKE_ARGS 
		-DBUILD_TESTS:BOOL=OFF
		-DBoost_INCLUDE_DIR:STRING=${Boost_INCLUDE_DIR}
		-DBUILD_SHARED_LIBS:BOOL=${ARG_BUILD_SHARED_LIBS}
)

OPTION(BUILD_App_DoseAcc "Determine if the demo application DoseAcc will be generated." ON)
IF(BUILD_App_DoseAcc)
	ADD_SUBDIRECTORY(DoseAcc)
ENDIF()

OPTION(BUILD_App_DoseMap "Determine if the application DoseMap will be generated." ON)
IF(BUILD_App_DoseMap)
	ADD_SUBDIRECTORY(DoseMap)
ENDIF()

OPTION(BUILD_App_DoseTool "Determine if the application DoseTool will be generated." ON)
IF(BUILD_App_DoseTool)
	ADD_SUBDIRECTORY(DoseTool)
ENDIF()

OPTION(BUILD_App_VoxelizerTool "Determine if the application VoxelizerTool will be generated." ON)
IF(BUILD_App_VoxelizerTool)
	ADD_SUBDIRECTORY(VoxelizerTool)
ENDIF()

OPTION(BUILD_App_BioModelCalc "Determine if the demo application BioModelCalc will be generated." ON)
IF(BUILD_App_BioModelCalc)
	ADD_SUBDIRECTORY(BioModelCalc)
ENDIF()