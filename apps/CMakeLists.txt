MESSAGE(STATUS "processing RTToolbox apps")

IF (WIN32)
	IF (MSVC_VERSION LESS 1800)
		MESSAGE(FATAL_ERROR "The apps of RTToolbox requires C++11 features (Visual Studio 2013 or higher).")
	ENDIF(MSVC_VERSION LESS 1800)
ENDIF(WIN32)


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
	IF(RTTB_VIRTUOS_SUPPORT AND BUILD_IO_Virtuos)
		ADD_SUBDIRECTORY(DoseAcc4V)
	ENDIF()
ENDIF()

OPTION(BUILD_App_DoseMap "Determine if the application DoseMap will be generated." ON)
IF(BUILD_App_DoseMap AND BUILD_IO_Virtuos)
	ADD_SUBDIRECTORY(DoseMap)
ENDIF()

OPTION(BUILD_App_DoseTool "Determine if the application DoseTool will be generated." ON)
IF(BUILD_App_DoseTool)
	ADD_SUBDIRECTORY(DoseTool)
	IF(RTTB_VIRTUOS_SUPPORT AND BUILD_IO_Virtuos)
		ADD_SUBDIRECTORY(DoseTool4V)
	ENDIF()
ENDIF()

OPTION(BUILD_App_VoxelizerTool "Determine if the application VoxelizerTool will be generated." ON)
IF(BUILD_App_VoxelizerTool)
	ADD_SUBDIRECTORY(VoxelizerTool)
ENDIF()

OPTION(BUILD_App_BioModelCalc "Determine if the demo application BioModelCalc will be generated." ON)
IF(BUILD_App_BioModelCalc)
	ADD_SUBDIRECTORY(BioModelCalc)
	IF(RTTB_VIRTUOS_SUPPORT AND BUILD_IO_Virtuos)
		ADD_SUBDIRECTORY(BioModelCalc4V)
	ENDIF()
ENDIF()