MESSAGE(STATUS "processing RTToolbox apps")

IF (BUILD_App_DoseTool OR BUILD_App_VoxelizerTool OR BUILD_App_BioModelCalc OR BUILD_App_DoseAcc)
	IF (WIN32)
		IF (MSVC_VERSION LESS 1800)
			MESSAGE(FATAL_ERROR "Some features of RTToolbox requires C++11 features that require Visual Studio 2013 or higher.")
		ENDIF(MSVC_VERSION LESS 1800)
	ENDIF(WIN32)
	
	#extract and build ArgumentParsingLib
	include(ExternalProject)
	message(STATUS "ArgumentParsingLib will be automatically downloaded and built.")
	set(ArgumentParsingLib_SOURCE_DIR "${CMAKE_BINARY_DIR}/external/ArgumentParsingLib-src")
	set(ArgumentParsingLib_BUILD_DIR "${CMAKE_BINARY_DIR}/external/ArgumentParsingLib-build")
	set(ArgumentParsingLib_CMAKE_DIR "${CMAKE_BINARY_DIR}/external/ArgumentParsingLib-cmake")
	
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
			-DBoost_DIR:STRING=${Boost_DIR}
	)
ENDIF()

OPTION(BUILD_App_DoseAcc "Determine if the application DoseAcc will be generated." OFF)
IF(BUILD_App_DoseAcc AND BUILD_IO_Virtuos)
	ADD_SUBDIRECTORY(DoseAcc)
ENDIF()

OPTION(BUILD_App_DoseMap "Determine if the application DoseMap will be generated." OFF)
IF(BUILD_App_DoseMap AND BUILD_IO_Virtuos)
	ADD_SUBDIRECTORY(DoseMap)
ENDIF()

OPTION(BUILD_App_DoseTool "Determine if the demo application DoseTool will be generated." OFF)
IF(BUILD_App_DoseTool)
	ADD_SUBDIRECTORY(DoseTool)
	IF(RTTB_VIRTUOS_SUPPORT AND BUILD_IO_Virtuos)
		ADD_SUBDIRECTORY(DoseTool4V)
	ENDIF()
ENDIF(BUILD_App_DoseTool)

OPTION(BUILD_App_VoxelizerTool "Determine if the application VoxelizerTool will be generated." OFF)
IF(BUILD_App_VoxelizerTool)
	ADD_SUBDIRECTORY(VoxelizerTool)
ENDIF(BUILD_App_VoxelizerTool)

OPTION(BUILD_App_BioModelCalc "Determine if the demo application BioModelCalc will be generated." OFF)
IF(BUILD_App_BioModelCalc)
	ADD_SUBDIRECTORY(BioModelCalc)
	IF(RTTB_VIRTUOS_SUPPORT AND BUILD_IO_Virtuos)
		ADD_SUBDIRECTORY(BioModelCalc4V)
	ENDIF()
ENDIF(BUILD_App_BioModelCalc)