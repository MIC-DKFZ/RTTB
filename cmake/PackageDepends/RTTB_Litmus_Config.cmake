#-----------------------------------------------------------------------------
# Litmus is built automatically. Just set include dir and link directories
#-----------------------------------------------------------------------------

set(Litmus_INCLUDE_DIRS "${LITMUS_SOURCE_DIR}/code/common;${LITMUS_SOURCE_DIR}/code/itk;${LITMUS_BUILD_DIR}")
set(Litmus_LIBRARY_DIRS "${LITMUS_BUILD_DIR}/bin")

IF ((BUILD_RTToolbox_io_Tester AND BUILD_IO_ITK) OR (BUILD_RTToolbox_interpolation_Tester AND BUILD_InterpolationMatchPointTransformation))
	SET(Litmus_ITK "LitmusITK")
ENDIF()

LIST(APPEND ALL_INCLUDE_DIRECTORIES ${Litmus_INCLUDE_DIRS})
LIST(APPEND ALL_LIBRARIES LitmusCommon ${Litmus_ITK})

LINK_DIRECTORIES(${Litmus_LIBRARY_DIRS}) 
