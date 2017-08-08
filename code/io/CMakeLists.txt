MESSAGE (STATUS "processing RTToolbox io")
ADD_SUBDIRECTORY (other)

OPTION(BUILD_IO_Dicom "Determine if the IO class wrappers for DICOM format will be generated." ON)
IF(BUILD_All_Modules OR BUILD_IO_Dicom)
	ADD_SUBDIRECTORY(dicom)
	OPTION(BUILD_IO_HELAX "Determine if the IO class wrappers for HELAX format will be generated." OFF)
	IF(BUILD_All_Modules OR BUILD_IO_HELAX)
		ADD_SUBDIRECTORY(helax)
		SET(BUILD_IO_HELAX ON CACHE BOOL ON FORCE)
	ENDIF()
ENDIF()

OPTION(BUILD_IO_ITK "Determine if the IO class wrappers for ITK image formats will be generated." OFF)
IF(BUILD_All_Modules OR BUILD_IO_ITK)
	ADD_SUBDIRECTORY(itk)
	SET(BUILD_IO_ITK ON CACHE BOOL ON FORCE)
ENDIF()

IF(BUILD_All_Modules OR BUILD_Models)
	ADD_SUBDIRECTORY(models)
ENDIF()

IF(BUILD_All_Modules OR (BUILD_IO_Dicom AND BUILD_IO_HELAX AND BUILD_IO_ITK))
	ADD_SUBDIRECTORY (utils)
ENDIF()

